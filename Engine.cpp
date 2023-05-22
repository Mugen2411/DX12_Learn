#include "Engine.h"
#include "DxGraphic.h"

namespace mugen_engine {

static Engine g_engine;
Engine& getIns() { return g_engine; }

void EnableDebugLayer() {
  ID3D12Debug* debugLayer = nullptr;
  auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));
  debugLayer->EnableDebugLayer();
  debugLayer->Release();
}

Engine::Engine():currentCbvSrvUavIndex(0){}

Engine::~Engine() {}

void Engine::Initialize(int width, int height, HWND hWnd) {
#ifdef _DEBUG
  EnableDebugLayer();
#endif
  _width = width;
  _height = height;
  _hwnd = hWnd;
  initDevice();
  initCommandList();
  initSwapChain(_width, _height, hWnd);
  initRenderTargetView();
  initFence();
  loadShader();
  initDescriptorHeap();
  initRootSignature();
  initViewPort(_width, _height);
  initScissorRect(_width, _height);
}

void Engine::ScreenFlip() {
  for (auto& i : _loaded_graphics) {
    i.second->Render();
  }
  D3D12_RESOURCE_BARRIER barrierDesc = {};
  barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barrierDesc.Transition.pResource = _backBuffers[_currentBackBufferIdx].Get();
  barrierDesc.Transition.Subresource = 0;
  barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
  barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
  _cmdList->ResourceBarrier(1, &barrierDesc);
  auto result = _cmdList->Close();
  ID3D12CommandList* _cmdLists[] = {_cmdList.Get()};
  _cmdQueue->ExecuteCommandLists(1, _cmdLists);
  _cmdQueue->Signal(_fence.Get(), ++_fenceval);
  if (_fence->GetCompletedValue() != _fenceval) {
    auto event = CreateEvent(nullptr, false, false, nullptr);
    _fence->SetEventOnCompletion(_fenceval, event);
    WaitForSingleObject(event, INFINITE);
    CloseHandle(event);
  }
  _cmdAllocator->Reset();
  _cmdList->Reset(_cmdAllocator.Get(), nullptr);
  for (auto& i : _loaded_graphics) {
    i.second->Clear();
  }
  _swapchain->Present(1, 0);
}

void Engine::Process() {
  _currentBackBufferIdx = _swapchain->GetCurrentBackBufferIndex();
  D3D12_RESOURCE_BARRIER barrierDesc = {};
  barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
  barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
  barrierDesc.Transition.pResource = _backBuffers[_currentBackBufferIdx].Get();
  barrierDesc.Transition.Subresource = 0;
  barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
  barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
  _cmdList->ResourceBarrier(1, &barrierDesc);
  auto rtvH = _rtvHeaps->GetCPUDescriptorHandleForHeapStart();
  rtvH.ptr += _currentBackBufferIdx * _dev->GetDescriptorHandleIncrementSize(
                                          D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  _cmdList->OMSetRenderTargets(1, &rtvH, true, nullptr);
  float clearColor[] = {0.2, 0.2, 0.2, 1.0};
  _cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
  _cmdList->RSSetViewports(1, &_viewport);
  _cmdList->RSSetScissorRects(1, &_scissorrect);
  _cmdList->SetGraphicsRootSignature(_rootsignature.Get());
  _cmdList->SetDescriptorHeaps(1, _descHeap.GetAddressOf());
}
void Engine::initDevice() {
  D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0,
                                D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};
  D3D_FEATURE_LEVEL fl;
  for (auto lv : levels) {
    if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&_dev)) == S_OK) {
      fl = lv;
      break;
    }
  }
  if (_dev == nullptr) {
    abort();
  }
  #ifdef _DEBUG
  auto flagsDXGI = DXGI_CREATE_FACTORY_DEBUG;
  CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(&_dxgiFactory));
  #else
  CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory));
  #endif
}
void Engine::initCommandList() {
  _dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                               IID_PPV_ARGS(_cmdAllocator.GetAddressOf()));
  _dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                          _cmdAllocator.Get(), nullptr,
                          IID_PPV_ARGS(_cmdList.GetAddressOf()));

  D3D12_COMMAND_QUEUE_DESC cmdQdesc = {};
  cmdQdesc.Flags = D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT;
  cmdQdesc.NodeMask = 0;
  cmdQdesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
  cmdQdesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  _dev->CreateCommandQueue(&cmdQdesc, IID_PPV_ARGS(_cmdQueue.GetAddressOf()));
  //_cmdAllocator->Reset();
}

void Engine::initSwapChain(int width, int height, HWND hwnd) {
  DXGI_SWAP_CHAIN_DESC scDesc = {};
  scDesc.BufferDesc.Width = width;
  scDesc.BufferDesc.Height = height;
  scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  scDesc.SampleDesc.Count = 1;
  scDesc.SampleDesc.Quality = 0;
  scDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
  scDesc.BufferCount = 2;
  scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  scDesc.OutputWindow = hwnd;
  scDesc.Windowed = true;

  auto res = _dxgiFactory->CreateSwapChain(
      _cmdQueue.Get(), &scDesc, (IDXGISwapChain**)_swapchain.GetAddressOf());
}
void Engine::initRenderTargetView() {
  D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};
  dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
  dhDesc.NodeMask = 0;
  dhDesc.NumDescriptors = 2;
  dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

  _dev->CreateDescriptorHeap(&dhDesc, IID_PPV_ARGS(_rtvHeaps.GetAddressOf()));

  DXGI_SWAP_CHAIN_DESC sdDesc = {};
  _swapchain->GetDesc(&sdDesc);

  _backBuffers.resize(sdDesc.BufferCount);
  auto hnd = _rtvHeaps->GetCPUDescriptorHandleForHeapStart();
  for (int i = 0; i < sdDesc.BufferCount; i++) {
    _swapchain->GetBuffer(i, IID_PPV_ARGS(_backBuffers[i].GetAddressOf()));
    hnd.ptr += i * _dev->GetDescriptorHandleIncrementSize(
                       D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    _dev->CreateRenderTargetView(_backBuffers[i].Get(), nullptr, hnd);
  }
}
void Engine::initFence() {
  _dev->CreateFence(_fenceval, D3D12_FENCE_FLAG_NONE,
                    IID_PPV_ARGS(_fence.GetAddressOf()));
}
void Engine::loadShader() {
  D3DCompileFromFile(L"BasicVertexShader.hlsl", nullptr,
                     D3D_COMPILE_STANDARD_FILE_INCLUDE, "BasicVS", "vs_5_0",
                     D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
                     _vsBlob.GetAddressOf(), _errorBlob.GetAddressOf());
  if (_vsBlob.Get() == nullptr) {
    std::string errstr;
    errstr.resize(_errorBlob->GetBufferSize());
    std::copy_n((char*)_errorBlob->GetBufferPointer(),
                _errorBlob->GetBufferSize(), errstr.begin());
    OutputDebugStringA(errstr.c_str());
  }
  D3DCompileFromFile(L"BasicPixelShader.hlsl", nullptr,
                     D3D_COMPILE_STANDARD_FILE_INCLUDE, "BasicPS", "ps_5_0",
                     D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
                     _psBlob.GetAddressOf(), _errorBlob.GetAddressOf());
}
ComPtr<ID3D12PipelineState> Engine::CreateGraphicPipelineState(D3D12_BLEND_DESC blD) {
  D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
  gpipeline.pRootSignature = _rootsignature.Get();
  gpipeline.VS.pShaderBytecode = _vsBlob->GetBufferPointer();
  gpipeline.VS.BytecodeLength = _vsBlob->GetBufferSize();
  gpipeline.PS.pShaderBytecode = _psBlob->GetBufferPointer();
  gpipeline.PS.BytecodeLength = _psBlob->GetBufferSize();

  gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
  gpipeline.RasterizerState.MultisampleEnable = false;
  gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
  gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
  gpipeline.RasterizerState.DepthClipEnable = true;

  gpipeline.BlendState = blD;

  gpipeline.InputLayout.pInputElementDescs = _inputLayout;
  gpipeline.InputLayout.NumElements = _countof(_inputLayout);
  gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

  gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  gpipeline.NumRenderTargets = 1;
  gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

  gpipeline.SampleDesc.Count = 1;
  gpipeline.SampleDesc.Quality = 0;

  ComPtr<ID3D12PipelineState> ret = nullptr;
  auto res = _dev->CreateGraphicsPipelineState(
      &gpipeline, IID_PPV_ARGS(ret.GetAddressOf()));
  return ret;
}
void Engine::initRootSignature() {
  D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};
  descTblRange[0].NumDescriptors = 1;
  descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
  descTblRange[0].BaseShaderRegister = 0;
  descTblRange[0].OffsetInDescriptorsFromTableStart =
      D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

  D3D12_ROOT_PARAMETER rootParam[2] = {};
  rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
  rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
  rootParam[0].DescriptorTable.pDescriptorRanges = &descTblRange[0];
  rootParam[0].DescriptorTable.NumDescriptorRanges = 1;

  D3D12_STATIC_SAMPLER_DESC smpD = {};
  smpD.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  smpD.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  smpD.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
  smpD.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
  smpD.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
  smpD.MaxLOD = D3D12_FLOAT32_MAX;
  smpD.MinLOD = 0.0f;
  smpD.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
  smpD.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

  D3D12_ROOT_SIGNATURE_DESC rsD = {};
  rsD.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
  rsD.pParameters = rootParam;
  rsD.NumParameters = 1;
  rsD.pStaticSamplers = &smpD;
  rsD.NumStaticSamplers = 1;

  ID3DBlob* rootSigBlob = nullptr;
  D3D12SerializeRootSignature(&rsD, D3D_ROOT_SIGNATURE_VERSION_1_0,
                              &rootSigBlob, _errorBlob.GetAddressOf());
  _dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
                            rootSigBlob->GetBufferSize(),
                            IID_PPV_ARGS(_rootsignature.GetAddressOf()));
  rootSigBlob->Release();
}
void Engine::initViewPort(int width, int height) {
  _viewport.Width = width;
  _viewport.Height = height;
  _viewport.TopLeftX = 0;
  _viewport.TopLeftY = 0;
  _viewport.MaxDepth = 1.0f;
  _viewport.MinDepth = 0.0f;
}
void Engine::initScissorRect(int width, int height) {
  _scissorrect.top = 0;
  _scissorrect.left = 0;
  _scissorrect.right = _scissorrect.left + width;
  _scissorrect.bottom = _scissorrect.top + height;
}
void Engine::initDescriptorHeap() {
  D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};
  dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  dhDesc.NodeMask = 0;
  dhDesc.NumDescriptors = 1024;
  dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  getIns().getDevice()->CreateDescriptorHeap(
      &dhDesc, IID_PPV_ARGS(_descHeap.GetAddressOf()));
}

void Engine::Execute() {}

std::shared_ptr<Graphic> Engine::LoadGraphic(std::string gid,
                                             std::wstring filepath, int width,
                                             int height, int divnum, int xnum,
                                             int ynum) {
  auto itr = _loaded_graphics[gid];
  if (!itr) _loaded_graphics[gid] = std::make_shared<Graphic>(filepath, width, height, divnum, xnum, ynum);
  itr = _loaded_graphics[gid];
  itr->Load();
  return itr;
}

std::shared_ptr<Graphic> Engine::LoadGraphic(std::string gid) {
  auto itr = _loaded_graphics[gid];
  if (!itr) abort();
  return itr;
}

}  // namespace mugen_engine
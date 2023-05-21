#pragma once
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <pix3.h>
#include <tchar.h>
#include <wrl/client.h>

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTex.lib")

namespace mugen_engine {
using Microsoft::WRL::ComPtr;
class Graphic;

class Engine {
 public:
  Engine();
  ~Engine();
  void Initialize(int width, int height, HWND hWnd);
  void ScreenFlip();
  void Process();
  ComPtr<ID3D12PipelineState> CreateGraphicPipelineState(D3D12_BLEND_DESC blD);
  ID3D12Device* getDevice() { return _dev.Get(); }
  ID3D12GraphicsCommandList* getCommandList() { return _cmdList.Get(); }
  UINT createSRV(ComPtr<ID3D12Resource> textureBuffer,
                 D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc) {
    _diff = _dev->GetDescriptorHandleIncrementSize(
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    _start = _descHeap->GetCPUDescriptorHandleForHeapStart();
    auto addr = _start;
    addr.ptr += currentCbvSrvUavIndex * _diff;
    _dev->CreateShaderResourceView(textureBuffer.Get(), &srvDesc, addr);
    return currentCbvSrvUavIndex++;
  }
  UINT createCBV(ComPtr<ID3D12Resource> constBuffer,
                 D3D12_CONSTANT_BUFFER_VIEW_DESC constDesc) {
    _diff = _dev->GetDescriptorHandleIncrementSize(
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    _start = _descHeap->GetCPUDescriptorHandleForHeapStart();
    auto addr = _start;
    addr.ptr += currentCbvSrvUavIndex * _diff;
    _dev->CreateConstantBufferView(&constDesc, addr);
    return currentCbvSrvUavIndex++;
  }
  void SetRootDescriptorTable(UINT index) {
    auto diff = _dev->GetDescriptorHandleIncrementSize(
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    auto start = _descHeap->GetGPUDescriptorHandleForHeapStart();
    start.ptr += diff * index;
    _cmdList->SetGraphicsRootDescriptorTable(0, start);
  }
  void Execute();
  std::shared_ptr<Graphic> LoadGraphic(std::string gid, std::wstring filepath, int width, int height, int divnum, int xnum, int ynum);
  std::shared_ptr<Graphic> LoadGraphic(std::string gid);

  int _width;
  int _height;

 private:
  ComPtr<ID3D12Device> _dev = nullptr;
  ComPtr<IDXGIFactory6> _dxgiFactory = nullptr;
  ComPtr<IDXGISwapChain4> _swapchain = nullptr;
  ComPtr<ID3D12CommandAllocator> _cmdAllocator = nullptr;
  ComPtr<ID3D12GraphicsCommandList> _cmdList = nullptr;
  ComPtr<ID3D12CommandQueue> _cmdQueue = nullptr;
  ComPtr<ID3D12RootSignature> _rootsignature = nullptr;
  ComPtr<ID3D12DescriptorHeap> _rtvHeaps = nullptr;
  std::vector<ComPtr<ID3D12Resource>> _backBuffers;
  ComPtr<ID3D12Fence> _fence = nullptr;
  UINT64 _fenceval = 0;
  D3D12_VIEWPORT _viewport = {};
  D3D12_RECT _scissorrect = {};
  UINT _currentBackBufferIdx;
  ComPtr<ID3D12DescriptorHeap> _descHeap = nullptr;
  UINT _diff;
  D3D12_CPU_DESCRIPTOR_HANDLE _start;

  int currentCbvSrvUavIndex;

  ComPtr<ID3DBlob> _vsBlob = nullptr;
  ComPtr<ID3DBlob> _psBlob = nullptr;
  ComPtr<ID3DBlob> _errorBlob = nullptr;

  D3D_FEATURE_LEVEL levels[4] = {D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0,
                                 D3D_FEATURE_LEVEL_11_1,
                                 D3D_FEATURE_LEVEL_11_0};

  D3D12_INPUT_ELEMENT_DESC _inputLayout[8] = {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
       D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
       0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
      {"MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
       D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
      {"MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
       D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
      {"MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32,
       D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
      {"MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48,
       D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
      {"BLENDLEVEL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1},
      {"DIVUV", 0, DXGI_FORMAT_R32G32_FLOAT, 1,
       D3D12_APPEND_ALIGNED_ELEMENT,
       D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1}};

  std::unordered_map<std::string, std::shared_ptr<Graphic>> _loaded_graphics;

  void initDevice();
  void initCommandList();
  void initSwapChain(int width, int height, HWND hwnd);
  void initRenderTargetView();
  void initFence();
  void loadShader();
  void initRootSignature();
  void initViewPort(int width, int height);
  void initScissorRect(int width, int height);
  void initDescriptorHeap();
};

Engine& getIns();
}  // namespace mugen_engine
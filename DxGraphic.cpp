#include "DxGraphic.h"

namespace mugen_engine {
Graphic::Graphic() : _texData(0), _width(32), _height(32) {}
void Graphic::Load() {
  D3D12_HEAP_PROPERTIES heapprop = {};
  heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
  heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

  D3D12_RESOURCE_DESC resdesc = {};
  resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  resdesc.Width = sizeof(_reserve_list[0]) * maxInstance;
  resdesc.Height = 1;
  resdesc.DepthOrArraySize = 1;
  resdesc.MipLevels = 1;
  resdesc.Format = DXGI_FORMAT_UNKNOWN;
  resdesc.SampleDesc.Count = 1;
  resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
  resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

  getIns().getDevice()->CreateCommittedResource(
      &heapprop, D3D12_HEAP_FLAG_NONE, &resdesc,
      D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
      IID_PPV_ARGS(_vertBuff.GetAddressOf()));

  _texData.resize(256 * 256);
  for (auto& rgba : _texData) {
    rgba.R = rand() % 256;
    rgba.G = rand() % 256;
    rgba.B = rand() % 256;
    rgba.A = 255;
  }

  DirectX::TexMetadata mt = {};
  DirectX::ScratchImage scImg = {};
  DirectX::LoadFromWICFile(L"M.png", DirectX::WIC_FLAGS_NONE, &mt, scImg);

  _width = mt.width;
  _height = mt.height;

  heapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
  heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
  heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

  resdesc.Width = mt.width;
  resdesc.Height = mt.height;
  resdesc.DepthOrArraySize = mt.depth;
  resdesc.Format = mt.format;
  resdesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
  resdesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

  auto res = getIns().getDevice()->CreateCommittedResource(
      &heapprop, D3D12_HEAP_FLAG_NONE, &resdesc,
      D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr,
      IID_PPV_ARGS(_texBuff.GetAddressOf()));

  _texBuff->WriteToSubresource(0, nullptr, scImg.GetImages()->pixels,
                               scImg.GetImages()->rowPitch, scImg.GetImages()->slicePitch);

  _gpipelineState = getIns().CreateGraphicPipelineState();

  initShaderResourceView();
}
void Graphic::Draw(float x, float y, float a, float ex) {
  DirectX::XMVECTOR base[4] = {{-_width, _height, 0.0f, 1.0f},
                               {-_width, -_height, 0.0f, 1.0f},
                               {_width, _height, 0.0f, 1.0f},
                               {_width, -_height, 0.0f, 1.0f}};
  Vertex input[4] = {{{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
                     {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                     {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
                     {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}};
  DirectX::XMMATRIX scl = DirectX::XMMatrixIdentity();
  DirectX::XMMATRIX trs = DirectX::XMMatrixIdentity();
  scl.r[0].m128_f32[0] = 2.0f / getIns()._width * ex;
  scl.r[1].m128_f32[1] = -2.0f / getIns()._height * ex;
  trs.r[3].m128_f32[0] = (x / getIns()._width) - 1.0f;
  trs.r[3].m128_f32[1] = -((y / getIns()._height) - 1.0f);
  DirectX::XMMATRIX rot = DirectX::XMMatrixRotationZ(a);
  DirectX::XMMATRIX mod = scl * rot * trs;
  for (int i = 0; i < 4; i++) {
    if (_reserve_list.size() >= maxInstance * 4) break;
    base[i] = DirectX::XMVector4Transform(base[i], mod);
    input[i].pos.x = base[i].m128_f32[0];
    input[i].pos.y = base[i].m128_f32[1];
    input[i].pos.z = base[i].m128_f32[2];
    _reserve_list.push_back(input[i]);
  }
}
void Graphic::Render() {
  if (_reserve_list.empty()) return;
  Vertex* vertMap = nullptr;
  _vertBuff->Map(0, nullptr, (void**)&vertMap);
  std::copy(std::begin(_reserve_list), std::end(_reserve_list), vertMap);
  _vertBuff->Unmap(0, nullptr);

  D3D12_VERTEX_BUFFER_VIEW vbView = {};
  vbView.BufferLocation = _vertBuff->GetGPUVirtualAddress();
  vbView.SizeInBytes = (sizeof(_reserve_list[0]) * _reserve_list.size());
  vbView.StrideInBytes = sizeof(_reserve_list[0]);

  getIns().SetRootDescriptorTable(srvIndex);
  getIns().getCommandList()->SetPipelineState(_gpipelineState.Get());
  getIns().getCommandList()->IASetPrimitiveTopology(
      D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  getIns().getCommandList()->IASetVertexBuffers(0, 1, &vbView);
  getIns().getCommandList()->DrawInstanced(_reserve_list.size(), 1, 0, 0);
}
void Graphic::initShaderResourceView() {
  D3D12_SHADER_RESOURCE_VIEW_DESC srvD = {};
  srvD.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvD.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
  srvD.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
  srvD.Texture2D.MipLevels = 1;

  srvIndex = getIns().createSRV(_texBuff, srvD);
}
}  // namespace mugen_engine
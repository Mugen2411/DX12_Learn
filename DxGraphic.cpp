#include "DxGraphic.h"

namespace mugen_engine {
Graphic::Graphic(std::wstring filepath, int width, int height, int divnum,
                 int xnum, int ynum)
    : _width(width), _height(height), _path(filepath), _divInfo{divnum, xnum, ynum} {}
void Graphic::Load() {
  D3D12_HEAP_PROPERTIES heapprop = {};
  heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
  heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
  heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

  D3D12_RESOURCE_DESC resdesc = {};
  resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
  resdesc.Width = sizeof(Vertex) * 4;
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

  resdesc.Width = sizeof(_reserve_list[0]) * maxInstance;
  getIns().getDevice()->CreateCommittedResource(
      &heapprop, D3D12_HEAP_FLAG_NONE, &resdesc,
      D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
      IID_PPV_ARGS(_constBuff[0].GetAddressOf()));
  getIns().getDevice()->CreateCommittedResource(
      &heapprop, D3D12_HEAP_FLAG_NONE, &resdesc,
      D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
      IID_PPV_ARGS(_constBuff[1].GetAddressOf()));

  DirectX::TexMetadata mt = {};
  DirectX::ScratchImage scImg = {};
  DirectX::LoadFromWICFile(_path.c_str(), DirectX::WIC_FLAGS_NONE, &mt, scImg);

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

  res = _texBuff->WriteToSubresource(0, nullptr, scImg.GetImages()->pixels,
                                     scImg.GetImages()->rowPitch,
                                     scImg.GetImages()->slicePitch);

  D3D12_BLEND_DESC blD = {};
  blD.AlphaToCoverageEnable = false;
  blD.IndependentBlendEnable = false;
  D3D12_RENDER_TARGET_BLEND_DESC rtbD = {};
  rtbD.BlendEnable = true;
  rtbD.BlendOp = D3D12_BLEND_OP_ADD;
  rtbD.BlendOpAlpha = D3D12_BLEND_OP_ADD;
  rtbD.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
  rtbD.SrcBlend = D3D12_BLEND_SRC_ALPHA;
  rtbD.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
  rtbD.SrcBlendAlpha = D3D12_BLEND_ONE;
  rtbD.DestBlendAlpha = D3D12_BLEND_ZERO;
  blD.RenderTarget[0] = rtbD;
  blD.RenderTarget[1] = rtbD;
  _gpipelineState[0] = getIns().CreateGraphicPipelineState(blD);
  rtbD.BlendEnable = true;
  rtbD.BlendOp = D3D12_BLEND_OP_ADD;
  rtbD.SrcBlend = D3D12_BLEND_SRC_ALPHA;
  rtbD.DestBlend = D3D12_BLEND_ONE;
  blD.RenderTarget[0] = rtbD;
  blD.RenderTarget[1] = rtbD;
  _gpipelineState[1] = getIns().CreateGraphicPipelineState(blD);

  initShaderResourceView();
}

// 0:ƒAƒ‹ƒtƒ@ 1:‰ÁŽZ
void Graphic::Draw(float x, float y, float a, float ex, int divuv, int blendType,
                   Color c) {
  if (_reserve_list[blendType].size() >= maxInstance) return;
  DirectX::XMMATRIX scl = DirectX::XMMatrixScaling(
      2.0f / getIns()._width * ex, -2.0f / getIns()._height * ex, 1.0f);
  float offx = x / getIns()._width * 2 - 1.0f;
  float offy = y / getIns()._height * 2 - 1.0f;
  DirectX::XMMATRIX trs = DirectX::XMMatrixTranslation(offx, -offy, 0);
  DirectX::XMMATRIX rot = DirectX::XMMatrixRotationZ(a);
  DirectX::XMMATRIX mod = rot * scl * trs;
  DirectX::XMFLOAT2 div;
  div.x = 1.0f / _divInfo.xnum * (divuv % _divInfo.xnum);
  div.y = 1.0f / _divInfo.ynum * (int)(divuv / _divInfo.xnum);
  InputPerInstance ret = {mod, {c.R, c.G, c.B, c.blendLevel}, div};
  _reserve_list[blendType].push_back(ret);
}
void Graphic::Render() {
  Vertex base[4] = {
      {{(float)-_width / 2, (float)_height / 2, 0.0f},
       {0.0f, 1.0f / _divInfo.ynum}},
      {{(float)-_width / 2, (float)-_height / 2, 0.0f}, {0.0f, 0.0f}},
      {{(float)_width / 2, (float)_height / 2, 0.0f},
       {1.0f / _divInfo.xnum, 1.0f / _divInfo.ynum}},
      {{(float)_width / 2, (float)-_height / 2, 0.0f},
       {1.0f / _divInfo.xnum, 0.0f}}};

  Vertex* vertMap = nullptr;
  _vertBuff->Map(0, nullptr, (void**)&vertMap);
  std::copy(std::begin(base), std::end(base), vertMap);
  _vertBuff->Unmap(0, nullptr);

  InputPerInstance* constMap[2] = {nullptr};
  _constBuff[0]->Map(0, nullptr, (void**)&constMap[0]);
  std::copy(std::begin(_reserve_list[0]), std::end(_reserve_list[0]),
            constMap[0]);
  _constBuff[0]->Unmap(0, nullptr);
  _constBuff[1]->Map(0, nullptr, (void**)&constMap[1]);
  std::copy(std::begin(_reserve_list[1]), std::end(_reserve_list[1]),
            constMap[1]);
  _constBuff[1]->Unmap(0, nullptr);

  D3D12_VERTEX_BUFFER_VIEW vbView[2][2] = {};
  vbView[0][0].BufferLocation = _vertBuff->GetGPUVirtualAddress();
  vbView[0][0].SizeInBytes = (sizeof(base[0]) * _countof(base));
  vbView[0][0].StrideInBytes = sizeof(base[0]);
  vbView[0][1].BufferLocation = _constBuff[0]->GetGPUVirtualAddress();
  vbView[0][1].SizeInBytes =
      sizeof(_reserve_list[0][0]) * _reserve_list[0].size();
  vbView[0][1].StrideInBytes = sizeof(_reserve_list[0][0]);
  vbView[1][0].BufferLocation = _vertBuff->GetGPUVirtualAddress();
  vbView[1][0].SizeInBytes = (sizeof(base[0]) * _countof(base));
  vbView[1][0].StrideInBytes = sizeof(base[0]);
  vbView[1][1].BufferLocation = _constBuff[1]->GetGPUVirtualAddress();
  vbView[1][1].SizeInBytes =
      sizeof(_reserve_list[1][0]) * _reserve_list[1].size();
  vbView[1][1].StrideInBytes = sizeof(_reserve_list[1][0]);

  if (!_reserve_list[0].empty()) {
    getIns().SetRootDescriptorTable(srvIndex);
    getIns().getCommandList()->IASetPrimitiveTopology(
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    getIns().getCommandList()->SetPipelineState(_gpipelineState[0].Get());
    getIns().getCommandList()->IASetVertexBuffers(0, 2, vbView[0]);
    getIns().getCommandList()->DrawInstanced(4, _reserve_list[0].size(), 0, 0);
  }
  if (!_reserve_list[1].empty()) {
    getIns().SetRootDescriptorTable(srvIndex);
    getIns().getCommandList()->IASetPrimitiveTopology(
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    getIns().getCommandList()->SetPipelineState(_gpipelineState[1].Get());
    getIns().getCommandList()->IASetVertexBuffers(0, 2, vbView[1]);
    getIns().getCommandList()->DrawInstanced(4, _reserve_list[1].size(), 0, 0);
  }
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
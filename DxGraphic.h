#pragma once
#include "Engine.h"

namespace mugen_engine {
class Graphic {
 public:
  Graphic();
  void Load();
  void Draw(float x, float y, float a, float ex);
  void Render();
  void Clear() { _reserve_list.clear(); }

 protected:
  struct Vertex {
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 uv;
  };
  struct TexRGBA {
    uint8_t R, G, B, A;
  };
  int _width;
  int _height;
  const UINT maxInstance = 16384;
  std::vector<Vertex> _reserve_list;
  std::vector<TexRGBA> _texData;
  ComPtr<ID3D12Resource> _vertBuff = nullptr;
  ComPtr<ID3D12Resource> _texBuff = nullptr;
  ComPtr<ID3D12PipelineState> _gpipelineState = nullptr;
  UINT srvIndex = 0;

  void initShaderResourceView();
};
}  // namespace mugen_engine
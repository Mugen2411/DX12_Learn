#pragma once
#include "Engine.h"

namespace mugen_engine {
class Graphic {
 public:
  Graphic(std::wstring filepath, int width, int height, int divnum, int xnum, int ynum);

  struct Color {
    float R, G, B, blendLevel;
  };

  void Load();
  void Draw(float x, float y, float a, float ex, float priority,int divuv = 0, int blendType = 0, Color c = {1.0f,1.0f,1.0f,1.0f});
  void Render();
  void Clear() {
    _reserve_list[0].clear();
    _reserve_list[1].clear();
  }

 protected:
  struct Vertex {
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 uv;
  };

  struct DivInfo {
    int all, xnum, ynum;
  } _divInfo;

  struct InputPerInstance {
    DirectX::XMMATRIX matrix;
    DirectX::XMFLOAT4 blendlevel;
    DirectX::XMFLOAT2 divuv;
  };

  int _width;
  int _height;
  std::wstring _path;
  const UINT maxInstance = 65535;
  std::vector<InputPerInstance> _reserve_list[2];
  ComPtr<ID3D12Resource> _vertBuff = nullptr;
  ComPtr<ID3D12Resource> _constBuff[2] = {nullptr};
  ComPtr<ID3D12Resource> _texBuff = nullptr;
  ComPtr<ID3D12PipelineState> _gpipelineState[2] = {nullptr};
  UINT srvIndex = 0;

  void initShaderResourceView();
};
const Graphic::Color MainColor[8] = {
    Graphic::Color{1.0f, 0.0f, 0.0f, 1.0f},
    Graphic::Color{1.0f, 0.5f, 0.0f, 1.0f},
    Graphic::Color{1.0f, 1.0f, 0.0f, 1.0f},
    Graphic::Color{0.0f, 1.0f, 0.0f, 1.0f},
    Graphic::Color{0.0f, 1.0f, 1.0f, 1.0f},
    Graphic::Color{1.0f, 0.0f, 1.0f, 1.0f},
    Graphic::Color{0.7f, 0.0f, 5.0f, 1.0f},
    Graphic::Color{0.5f, 0.5f, 0.5f, 1.0f}
};
Graphic::Color GetColorByCode(int code);
}  // namespace mugen_engine
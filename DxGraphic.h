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
  void Render(int BlendType);
  void Clear() {
    for (auto &i : _reserve_list) {
      i.clear();
      i.reserve(maxInstance);
    }
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
  const UINT maxInstance = 32768;
  std::vector<InputPerInstance> _reserve_list[3];
  ComPtr<ID3D12Resource> _vertBuff = nullptr;
  ComPtr<ID3D12Resource> _constBuff[3] = {nullptr};
  ComPtr<ID3D12Resource> _texBuff = nullptr;
  ComPtr<ID3D12PipelineState> _gpipelineState[3] = {nullptr};
  UINT srvIndex = 0;

  void initShaderResourceView();
};
const Graphic::Color MainColor[8] = {
    Graphic::Color{1.0f, 0.0f, 0.0f, 1.0f},
    Graphic::Color{1.0f, 0.5f, 0.0f, 1.0f},
    Graphic::Color{1.0f, 1.0f, 0.0f, 1.0f},
    Graphic::Color{0.0f, 1.0f, 0.0f, 1.0f},
    Graphic::Color{0.0f, 1.0f, 8.0f, 1.0f},
    Graphic::Color{0.0f, 0.3f, 1.0f, 1.0f},
    Graphic::Color{0.7f, 0.0f, 6.0f, 1.0f},
    Graphic::Color{0.5f, 0.5f, 0.5f, 1.0f}
};
Graphic::Color GetColorByCode(int code);
}  // namespace mugen_engine
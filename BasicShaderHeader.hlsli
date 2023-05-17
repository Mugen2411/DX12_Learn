struct Input {
  float4 pos : POSITION;
  float2 uv : TEXCOORD;
  float4x4 mat : MATRIX;
  float4 blendlevel : BLENDLEVEL;
  float2 divuv : DIVUV;
  uint id : SV_InstanceID;
};

struct Output {
  float4 pos : SV_POSITION;
  float2 uv : TEXCOORD;
  float2 divuv : DIVUV;
  float4 blendlevel : blendlevel;
};
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
//cbuffer cbuff0 : register(b0) { matrix mat; };
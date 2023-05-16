struct Input {
  float4 pos : POSITION;
  float2 uv : TEXCOORD;
  //uint inst : SV_InstanceID;
};

struct Output {
  float4 pos : SV_POSITION;
  float2 uv : TEXCOORD;
};
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
//cbuffer cbuff0 : register(b0) { matrix mat; };
#include "BasicShaderHeader.hlsli"

int Seed;

float GetRandomNumber(float2 texCoord, int Seed) {
  return frac(sin(dot(texCoord.xy, float2(12.9898, 78.233)) + Seed) *
              43758.5453);
}

float4 BasicPS(Output input) : SV_TARGET {
  float4 col = tex.Sample(smp, input.uv + input.divuv);
  [branch] if (col.w == 0.0) { discard; }
  return float4(col*input.blendlevel);
}
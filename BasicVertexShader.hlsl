#include "BasicShaderHeader.hlsli"

Output BasicVS(Input input){
  Output output;
  output.pos = mul(input.mat, input.pos);
  output.uv = input.uv;
  output.divuv = input.divuv;
  output.blendlevel = input.blendlevel;
  return output;
}
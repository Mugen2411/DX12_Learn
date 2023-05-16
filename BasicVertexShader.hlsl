#include "BasicShaderHeader.hlsli"

Output BasicVS(Input input){
  Output output;
  output.pos = mul(input.mat, input.pos);
  output.uv = input.uv;
  return output;
}
#include "BasicShaderHeader.hlsli"

Output BasicVS(Input input){
  Output output;
  output.pos = input.pos;
  output.uv = input.uv;
  return output;
}
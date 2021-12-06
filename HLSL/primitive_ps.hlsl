#include "Primitive.hlsli"

float4 main(PSInput input) : SV_TARGET
{
    return input.Color;
}
#include "Debug.hlsli"

PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;
    output.position = mul(input.position, worldViewProjection);
    output.color = color;

    return output;
}
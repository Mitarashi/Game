#include "ImGui.hlsli"

PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;
    output.Position = mul(input.Position, worldViewProjection);
    output.Texcoord = input.Texcoord;
    output.Color = input.Color;

    return output;
}
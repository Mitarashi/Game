#include "Sprite.hlsli"

PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;
    output.Position = float4(input.Position, 1);
    output.Color = input.Color;
    output.Texcoord = input.Texcoord;

    return output;
}

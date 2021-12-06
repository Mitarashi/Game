#include "Primitive.hlsli"

PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;
    output.Position = input.Position;
    output.Color = input.Color;
    return output;
}
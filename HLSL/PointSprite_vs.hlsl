#include "PointSprite.hlsli"

GSInput main(VSInput input)
{
	GSInput output = (GSInput)0;

	output.Position = input.Position;
	output.Normal = input.Normal;
	output.Size = input.Size;
	output.Color = input.Color;

	return output;
}
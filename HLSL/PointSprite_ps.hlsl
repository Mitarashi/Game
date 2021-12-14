#include "PointSprite.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	float4 color = DiffuseTexture.Sample(DecalSampler, input.Texcoord) * input.Color;

	return color;
}
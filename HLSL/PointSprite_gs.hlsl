#include "PointSprite.hlsli"

[maxvertexcount(4)]
void main(point GSInput input[1],
	inout TriangleStream<PSInput> particleStream)
{
	float4 Position = mul(view, float4(input[0].Position, 1.0));

	float width = input[0].Size.x;
	float height = input[0].Size.y;
    uint type = input[0].Color.r;
	float u = (type % 4) * 0.25f;
	float v = (type / 4) * 0.25f;

	float4 Positions[4] =
	{
		{ Position + float4(-width,  height, 0, 0) }, // ç∂è„
		{ Position + float4( width,  height, 0, 0) }, // âEè„
		{ Position + float4(-width, -height, 0, 0) }, // ç∂â∫
		{ Position + float4( width, -height, 0, 0) }, // âEâ∫
	};

	float2 texturePositions[4] =
	{
		{ u, 		 v		   },
		{ u + 0.25f, v		   },
		{ u,		 v + 0.25f },
		{ u + 0.25f, v + 0.25f }
	};

	for (int i = 0; i < 4; i++)
	{
		PSInput output = (PSInput)0;
		output.Color = float4(1, 1, 1, input[0].Color.a);
		output.Position = mul(projection, Positions[i]);
		output.Texcoord = texturePositions[i];
		particleStream.Append(output);
	}

	particleStream.RestartStrip();
}
#include "ImGui.hlsli"

float4 main(PSInput input) : SV_TARGET
{
    return DeffuseTexture.Sample(DecalSampler, input.Texcoord) * input.Color;
}
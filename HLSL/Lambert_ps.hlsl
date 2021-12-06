#include "Lambert.hlsli"

float4 main(PSInput input) : SV_TARGET
{
    return DiffuseTexture.Sample(DecalSampler, input.texcoord) * input.color;
}
#include "sprite.hlsli"

// �s�N�Z���V�F�[�_�[
float4 main(PSInput input) : SV_TARGET0
{
    float4 color = DeffuseTexture.Sample(DecalSampler, input.Texcoord) * input.Color;

    return color;
}


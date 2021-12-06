#include "sprite.hlsli"

// ピクセルシェーダー
float4 main(PSInput input) : SV_TARGET0
{
    float4 color = DeffuseTexture.Sample(DecalSampler, input.Texcoord) * input.Color;

    return color;
}


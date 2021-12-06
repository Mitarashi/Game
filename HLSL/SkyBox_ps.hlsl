#include "SkyBox.hlsli"

// 視線ベクトルを正距円筒図法に則ったUV座標に変換
float2 EquirectangularProjection(float3 v)
{
    static const float PI = 3.141592654f;
    float2 tex = (float2) 0;
    tex.x = 1.0f - ((1.0f / (2 * PI)) * atan2(v.z, v.x) + 0.5f);
    tex.y = -(1.0f / PI) * atan2(v.y, length(v.xz)) + 0.5f;
    return tex;
}

// ピクセルシェーダー
float4 main(PSInput input) : SV_TARGET0
{
    float3 vec = normalize(input.WorldPos.xyz - eyePos.xyz);
    return DiffuseTexture.Sample(DecalSampler, EquirectangularProjection(vec));
}
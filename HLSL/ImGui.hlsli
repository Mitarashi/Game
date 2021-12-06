// テクスチャ
Texture2D DeffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

// データフォーマット
struct VSInput
{
    float4 Position : POSITION;
    float2 Texcoord : TEXCOORD;
    float4 Color    : COLOR;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD;
    float4 Color    : COLOR;
};

// グローバル変数
cbuffer ConstantBuffer : register(b0)
{
    row_major float4x4 worldViewProjection;
};
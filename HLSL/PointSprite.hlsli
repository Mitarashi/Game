// テクスチャ・サンプラ
Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

// データフォーマット
struct VSInput
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 Size     : TEXCOORD;
    float4 Color    : COLOR;
};

struct GSInput
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 Size     : TEXCOORD;
    float4 Color    : COLOR;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD;
    float4 Color    : COLOR0;
};

// 定数バッファ
cbuffer ConstantBuffer : register(b0)
{
    matrix view;
    matrix projection;
};
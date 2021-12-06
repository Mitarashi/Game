// テクスチャ
Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

// グローバル変数
cbuffer CbScene : register(b0)
{
    row_major float4x4 viewProjection;
    float4 lightDirection;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
    row_major float4x4 boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
    float4 materialColor;
};

// データフォーマット
struct VSInput
{
    float4 position     : POSITION;
    float3 normal       : NORMAL;
    float3 tangent      : TANGENT;
    float2 texcoord     : TEXCOORD;
    float4 color        : COLOR;
    float4 boneWeights  : WEIGHTS;
    uint4 boneIndices   : BONES;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color    : COLOR;
};
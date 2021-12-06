// グローバル変数
cbuffer CbMesh : register(b0)
{
    row_major float4x4 worldViewProjection;
    float4 color;
};

// データフォーマット
struct VSInput
{
    float4 position : POSITION;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


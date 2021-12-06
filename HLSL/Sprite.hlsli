// �e�N�X�`��
Texture2D DeffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

// �f�[�^�t�H�[�}�b�g
struct VSInput
{
    float3 Position : POSITION;
    float2 Texcoord : TEXCOORD;
    float4 Color    : COLOR;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 Texcoord : TEXCOORD;
    float4 Color    : COLOR;
};
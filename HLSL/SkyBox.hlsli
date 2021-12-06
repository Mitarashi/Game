// �e�N�X�`��
Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

// �O���[�o���ϐ�
cbuffer CBPerFrame : register(b2)
{
    matrix inverseView;
    matrix inverseProjection;
    float4 eyePos;
};

// �f�[�^�t�H�[�}�b�g
struct VSInput
{
    float3 Position : POSITION;
    float2 Tex      : TEXCOORD;
    float4 Color    : COLOR;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 WorldPos : POSITION;
};
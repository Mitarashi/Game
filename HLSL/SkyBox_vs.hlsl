#include "SkyBox.hlsli"

// ���_�V�F�[�_�[
PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;

    output.Position = float4(input.Position.xyz, 1);

    // ���[���h�ϊ�
    float4 position = mul(inverseProjection, float4(input.Position.xyz, 1));
    output.WorldPos = mul(inverseView, position / position.w).xyz;
	return output;
}
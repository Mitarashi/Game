#include "NoLight.hlsli"

// 頂点シェーダー
PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;

    float3 position = (float3) 0;
    float3 normal = (float3) 0;
    for (int i = 0; i < 4; i++)
    {
        position += (input.boneWeights[i] * mul(input.position, boneTransforms[input.boneIndices[i]])).xyz;
        normal += (input.boneWeights[i] * mul(float4(input.normal.xyz, 0), boneTransforms[input.boneIndices[i]])).xyz;
    }

    output.position = mul(float4(position, 1.0f), viewProjection);
    output.texcoord = input.texcoord;
    output.color = materialColor;
    return output;
}
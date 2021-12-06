#include "Lambert.hlsli"

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

    float3 Normal = normalize(normal);
    float3 LightDirection = normalize(-lightDirection.xyz);
    float vec = dot(LightDirection, Normal);
    float power = max(0, vec) * 0.5f + 0.5f;

    output.color.rgb = input.color.rgb * materialColor.rgb * power;
    output.color.a = input.color.a * materialColor.a;
    output.texcoord = input.texcoord;
    return output;
}

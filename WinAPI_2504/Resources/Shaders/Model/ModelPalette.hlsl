#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangentBlend input)
{
    LightPixelInput output;
    matrix transform = mul(SkinWorld(input.indices, input.weights), world);
    output.pos = mul(input.pos, transform);

    output.worldPos = output.pos.xyz;
    output.viewPos = invView._41_42_43;

    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.normal = mul(input.normal, (float3x3) transform);
    output.tangent = mul(input.tangent, (float3x3) transform);
    output.binormal = cross(output.normal, output.tangent);

    output.uv = input.uv;
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    // ?? 팔레트 텍스처에서 색상 샘플링
    float4 color = diffuseMap.Sample(samplerState, input.uv);

    // 필요시 LowPoly 느낌용 단순 조명 (선택)
    // float3 L = normalize(-lights[0].direction);
    // float NdotL = saturate(dot(normalize(input.normal), L));
    // color.rgb *= (0.3 + 0.7 * NdotL);

    return color;
}

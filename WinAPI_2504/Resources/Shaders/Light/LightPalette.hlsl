#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)
{
    LightPixelInput output;

    // 기본 월드-뷰-프로젝션 변환 (동일)
    output.pos = mul(input.pos, world);
    output.worldPos = output.pos.xyz;
    output.viewPos = invView._41_42_43;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    // 노멀/탠전트 계산은 그대로 (필요 시 생략 가능)
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    // ?? 팔레트 텍스처 색상 그대로 사용
    float4 color = diffuseMap.Sample(samplerState, input.uv);

    // LowPoly 스타일: 감마 조정 또는 플랫 셰이딩 효과를 주려면 이렇게
    // color.rgb = pow(color.rgb, 1.0 / 2.2); // 필요 시 감마 보정
    
    return color;
}

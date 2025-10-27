#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)
{
    LightPixelInput output;

    // �⺻ ����-��-�������� ��ȯ (����)
    output.pos = mul(input.pos, world);
    output.worldPos = output.pos.xyz;
    output.viewPos = invView._41_42_43;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    // ���/����Ʈ ����� �״�� (�ʿ� �� ���� ����)
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET
{
    // ?? �ȷ�Ʈ �ؽ�ó ���� �״�� ���
    float4 color = diffuseMap.Sample(samplerState, input.uv);

    // LowPoly ��Ÿ��: ���� ���� �Ǵ� �÷� ���̵� ȿ���� �ַ��� �̷���
    // color.rgb = pow(color.rgb, 1.0 / 2.2); // �ʿ� �� ���� ����
    
    return color;
}

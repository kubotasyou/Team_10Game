cbuffer cbuff0 : register(b0)
{
	float4 color;//�F
	matrix mat;//3D�ϊ��s��
}

//���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};
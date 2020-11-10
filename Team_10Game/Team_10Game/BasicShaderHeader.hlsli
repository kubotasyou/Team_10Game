cbuffer cbuff0 : register(b0)
{
	float4 color;//色
	matrix mat;//3D変換行列
}

//頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};
#include "SpriteShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	//ピクセルシェーダーに渡す値
	VSOutput output;
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}
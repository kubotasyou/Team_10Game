#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float4 color : COLOR, float scale : TEXCOORD)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.pos = pos;
	output.color = color;
	output.scale = scale;
	return output;
}
cbuffer cbuff0 : register(b0)
{
	matrix mat; // ３Ｄ変換行列
	matrix matBillboard;//ビルボード行列
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : SV_POSITION; //頂点座標
	float4 color : COLOR;//カラー
	float scale : TEXCOORD;//スケール
};

struct GSOutput
{
	float4 svpos : SV_POSITION;//システム用頂点座標
	float4 color : COLOR;
	float2 uv : TEXCOORD;//uv値
};
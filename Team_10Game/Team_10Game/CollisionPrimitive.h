#pragma once
#include <DirectXMath.h>

//当たり判定の形

struct Sphere
{
	//中心座標(センターのwは1)
	DirectX::XMVECTOR center = { 0, 0, 0, 1 };

	//半径
	float radius = 1.0f;
};

struct Plane
{
	//法線ベクトル(wは0)
	DirectX::XMVECTOR normal = { 0, 1, 0, 0 };

	//原点(0, 0, 0)からの距離
	float distance = 0.0f;
};

struct Line
{
	//始点座標
	DirectX::XMVECTOR start = { 0, 0, 0, 1 };
	//終点座標
	DirectX::XMVECTOR end = { 1, 0, 0, 1 };
};

//矩形
struct Rectangles
{
	//原点
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float width =  10.0f;
	float height = 10.0f;
	float okuyuki = 10.0f;
	
};

//点
struct Dot
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};
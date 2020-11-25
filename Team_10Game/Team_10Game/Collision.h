#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Collision
{
public:
	//円と円の衝突判定
	static bool CircleToCircle(XMFLOAT3 centerPosA, int radiusA,
		XMFLOAT3 centerPosB, int radiusB);

	//円と平面
	//distanceは原点からの距離であり、上に行ったら+、下に行ったら-になる
	static bool CheckSphereToPanel(XMFLOAT3 sphereCenter, float radius,
		XMFLOAT3 normal, float distance);
};


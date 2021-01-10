#pragma once

#include "CollisionPrimitive.h"

#include <DirectXMath.h>

using namespace DirectX;

class Collision
{
public:
	//円と円の衝突判定
	static bool SphereToSphere(
		const Sphere& sphere1,
		const Sphere& sphere2
	);

	//円と平面
	//distanceは原点からの距離であり、上に行ったら+、下に行ったら-になる
	static bool CheckSphereToPanel(XMFLOAT3 sphereCenter, float radius,
		XMFLOAT3 normal, float distance);

	//closest = 最近接点
    //点と三角形の最近点を求める(点・三角形・最近接点)
	static void ClosestPtPointToTriangle(
		XMFLOAT3 point,
		XMFLOAT3 triangleP0,
		XMFLOAT3 triangleP1,
		XMFLOAT3 triangleP2,
		XMFLOAT3 closester
	);

	//線と法線つkの三角形の当たり判定チェック(球・三角形・最近接点)
	static bool CheckSphereToTriangle(
		XMFLOAT3 sphereCenter,
		float radius,
		XMFLOAT3 triangleP0,
		XMFLOAT3 triangleP1,
		XMFLOAT3 triangleP2,
		XMVECTOR inter = XMVectorSet(0, 0, 0, 0)
	);
};


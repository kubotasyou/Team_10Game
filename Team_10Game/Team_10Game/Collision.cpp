#include "Collision.h"

bool Collision::CircleToCircle(XMFLOAT3 centerPosA, int radiusA, XMFLOAT3 centerPosB, int radiusB)
{
	//auto x = abs((centerPosA.x + radiusA) - (centerPosB.x + radiusB));
	//auto y = abs((centerPosA.y + radiusA) - (centerPosB.y + radiusB));
	//auto z = abs((centerPosA.z + radiusA) - (centerPosB.z + radiusB));

	auto test = ((centerPosB.x - centerPosA.x) *(centerPosB.x - centerPosA.x)) +
		        ((centerPosB.y - centerPosA.y) *(centerPosB.y - centerPosA.y)) +
		        ((centerPosB.z - centerPosA.z) *(centerPosB.z - centerPosA.z));

	float radius = (radiusA + radiusB) * (radiusA + radiusB);

	//これで当たってる
	if (test >= radius)
	{
		return false;
	}

	return true;
}

bool Collision::CheckSphereToPanel(XMFLOAT3 sphereCenter, float radius,
	XMFLOAT3 normal, float distance)
{
	XMVECTOR sphere = XMVectorSet(sphereCenter.x, sphereCenter.y, sphereCenter.z, 1.0f);
	XMVECTOR panel = XMVectorSet(normal.x, normal.y, normal.z, 0.0f);

	XMVECTOR distV = XMVector3Dot(sphere, panel);

	//法線ベクトルを決めることによって、平面が定義される。
	//distanceは法線ベクトル方向に動かした大きさ。
	float dist = distV.m128_f32[0] - distance;

	////fabsf(絶対値の結果をfloatで返す)
	//float absDist = fabsf(dist);
	//if (absDist > radius)
	//{
	//	return false;
	//}

	//距離の絶対値が、半径よりも大きければ当たっていない。
	if (fabsf(dist) > radius)
	{
		return false;
	}

	return true;
}

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

	//����œ������Ă�
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

	//�@���x�N�g�������߂邱�Ƃɂ���āA���ʂ���`�����B
	//distance�͖@���x�N�g�������ɓ��������傫���B
	float dist = distV.m128_f32[0] - distance;

	////fabsf(��Βl�̌��ʂ�float�ŕԂ�)
	//float absDist = fabsf(dist);
	//if (absDist > radius)
	//{
	//	return false;
	//}

	//�����̐�Βl���A���a�����傫����Γ������Ă��Ȃ��B
	if (fabsf(dist) > radius)
	{
		return false;
	}

	return true;
}

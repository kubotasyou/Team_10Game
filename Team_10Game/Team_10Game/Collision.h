#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Collision
{
public:
	//�~�Ɖ~�̏Փ˔���
	static bool CircleToCircle(XMFLOAT3 centerPosA, int radiusA,
		XMFLOAT3 centerPosB, int radiusB);

	//�~�ƕ���
	//distance�͌��_����̋����ł���A��ɍs������+�A���ɍs������-�ɂȂ�
	static bool CheckSphereToPanel(XMFLOAT3 sphereCenter, float radius,
		XMFLOAT3 normal, float distance);
};


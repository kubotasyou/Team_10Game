#pragma once

#include "CollisionPrimitive.h"

#include <DirectXMath.h>

using namespace DirectX;

class Collision
{
public:
	//�~�Ɖ~�̏Փ˔���
	static bool SphereToSphere(
		const Sphere& sphere1,
		const Sphere& sphere2
	);

	//�~�ƕ���
	//distance�͌��_����̋����ł���A��ɍs������+�A���ɍs������-�ɂȂ�
	static bool CheckSphereToPanel(XMFLOAT3 sphereCenter, float radius,
		XMFLOAT3 normal, float distance);

	//closest = �ŋߐړ_
    //�_�ƎO�p�`�̍ŋߓ_�����߂�(�_�E�O�p�`�E�ŋߐړ_)
	static void ClosestPtPointToTriangle(
		XMFLOAT3 point,
		XMFLOAT3 triangleP0,
		XMFLOAT3 triangleP1,
		XMFLOAT3 triangleP2,
		XMFLOAT3 closester
	);

	//���Ɩ@����k�̎O�p�`�̓����蔻��`�F�b�N(���E�O�p�`�E�ŋߐړ_)
	static bool CheckSphereToTriangle(
		XMFLOAT3 sphereCenter,
		float radius,
		XMFLOAT3 triangleP0,
		XMFLOAT3 triangleP1,
		XMFLOAT3 triangleP2,
		XMVECTOR inter = XMVectorSet(0, 0, 0, 0)
	);
};


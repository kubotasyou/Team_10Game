#include "Collision.h"

bool Collision::SphereToSphere(const Sphere & sphere1, const Sphere & sphere2)
{
	float x = (sphere2.center.m128_f32[0] - sphere1.center.m128_f32[0]) * (sphere2.center.m128_f32[0] - sphere1.center.m128_f32[0]);
	float y = (sphere2.center.m128_f32[1] - sphere1.center.m128_f32[1]) * (sphere2.center.m128_f32[1] - sphere1.center.m128_f32[1]);
	float z = (sphere2.center.m128_f32[2] - sphere1.center.m128_f32[2]) * (sphere2.center.m128_f32[2] - sphere1.center.m128_f32[2]);

	float rad2 = (sphere1.radius + sphere2.radius) * (sphere1.radius + sphere2.radius);

	if (x + y + z >= rad2)
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

void Collision::ClosestPtPointToTriangle(XMFLOAT3 point, XMFLOAT3 triangleP0, XMFLOAT3 triangleP1, XMFLOAT3 triangleP2, XMFLOAT3 closester)
{
	using namespace DirectX;

	XMVECTOR pt = XMVectorSet(point.x, point.y, point.z, 1);
	XMVECTOR p0 = XMVectorSet(triangleP0.x, triangleP0.y, triangleP0.z, 1);
	XMVECTOR p1 = XMVectorSet(triangleP1.x, triangleP1.y, triangleP1.z, 1);
	XMVECTOR p2 = XMVectorSet(triangleP2.x, triangleP2.y, triangleP2.z, 1);
	XMVECTOR closest = XMVectorSet(closester.x, closester.y, closester.z, 0);

	XMVECTOR p0_p1 = p1 - p0; //p1��p0�̕ӂ̃x�N�g���@�Ӈ@
	XMVECTOR p0_p2 = p2 - p0; //p2��p0�̕ӂ̃x�N�g���@�ӇA
	XMVECTOR p1_p2 = p2 - p1; //p2��p1�̕ӂ̃x�N�g���@�ӇB
	XMVECTOR p0_pt = pt - p0;       //�_��p0�̒����̃x�N�g��
	XMVECTOR p1_pt = pt - p1;       //�_��p1�̒����̃x�N�g��
	XMVECTOR p2_pt = pt - p2;       //�_��p2�̒����̃x�N�g��

	//���ς́AVECTOR�^�ŕԂ��Ă͂��邯�ǁAxyz�ǂ�ɂ�����Ȃ��l�������Ă���̂Ŏ���float�^���Ԃ��Ă�B
	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);//�Ӈ@�ƁA�_�Ƃ̋����̓���
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);//�ӇA�ƁA�_�Ƃ̋����̓���

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);//�Ӈ@�ƁA�_�Ƃ̋����̓���
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);//�ӇA�ƁA�_�Ƃ̋����̓���
	//XMVECTOR d4 = XMVector3Dot(p1_p2, p1_pt);//�ӇB�ƁA�_�Ƃ̋����̓���

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);//�Ӈ@�ƁA�_�Ƃ̋����̓���
	//XMVECTOR d5 = XMVector3Dot(p1_p2, p2_pt);//�Ӈ@�ƁA�_�Ƃ̋����̓���
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);//�ӇA�ƁA�_�Ƃ̋����̓���

	//�Ӈ@�ƁA�_�Ƃ̋����̓��� ���}�C�i�X && �ӇA�ƁA�_�Ƃ̋����̓��� ���}�C�i�X�̎�
	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		//p0���ߐړ_�ƂȂ�B
		closest = p0;
		return;
	}

	//-----------------------------------------------------------------

	//point��"p1"�̊O���̒��_�̈�̒��ɂ���̂��ǂ����`�F�b�N

	//
	if (d3.m128_f32[0] >= 0.0f && /*d4.m128_f32[0] <= 0.0f*/ d4.m128_f32[0] <= d3.m128_f32[0])
	{
		//p1���ڋߓ_�ƂȂ�
		closest = p1;
		return;
	}

	//-----------------------------------------------------------------

	//point��"p2"�̊O���̒��_�̈�̒��ɂ���̂��ǂ����`�F�b�N

	//
	if (d5.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0] /*d6.m128_f32[0] >= 0.0f*/)
	{
		//p2���ڋߓ_�ƂȂ�
		closest = p2;
		return;
	}

	//-----------------------------------------------------------------

	//point���Ӈ@�̕ӗ̈�̒��ɂ��邩�ǂ������`�F�b�N���A
	//�����point�̕Ӈ@��ɑ΂���ˉe��Ԃ��B

	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];

	//XMVECTOR vc = XMVector3Dot(XMVector3Cross(p0_p1, p0_p2), XMVector3Cross(p0_pt, p1_pt));

	if (vc/*.m128_f32[0]*/ <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		//�Ӈ@���ڋߓ_�ƂȂ�
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		closest = p0 + v * p0_p1;
		return;
	}

	//---------------------------------------------------------------------------

	//point���ӇA�̕ӗ̈�̒��ɂ��邩�ǂ������`�F�b�N���A
	//�����point�̕ӇA��ɑ΂���ˉe��Ԃ��B

	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];

	//XMVECTOR vb = XMVector3Dot(XMVector3Cross(p1_p2, p0_p1), XMVector3Cross(p1_pt, p2_pt));

	if (vb/*.m128_f32[0]*/ <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		//�ӇA���ڋߓ_�ƂȂ�
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		closest = p0 + w * p0_p2;
		return;
	}

	//---------------------------------------------------------------------------

	//point���ӇB�̕ӗ̈�̒��ɂ��邩�ǂ������`�F�b�N���A
	//�����point��p1_p2��ɑ΂���ˉe��Ԃ��B

	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];

	//XMVECTOR va = XMVector3Dot(XMVector3Cross(p0_p2, p1_p2), XMVector3Cross(p2_pt, p0_pt));

	if (va/*.m128_f32[0] */<= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) <= 0.0f)
	{
		//p1_p2�̕ӗ̈悪�ڋߓ_�ƂȂ�
		float x = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		closest = p1 + x * p1_p2;
		return;
	}

	float denom = 1.0f / (va/*.m128_f32[0]*/ + vb/*.m128_f32[0] */+ vc/*.m128_f32[0]*/);
	float v = vb/*.m128_f32[0]*/ * denom;
	float w = vc/*.m128_f32[0]*/ * denom;
	closest = p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphereToTriangle(XMFLOAT3 sphereCenter, float radius, XMFLOAT3 triangleP0, XMFLOAT3 triangleP1, XMFLOAT3 triangleP2, XMVECTOR inter)
{
	//���������Ƃ����ꏊ�Ƃ̍ŋߐړ_
	XMFLOAT3 point = { 0,0,0 };
	ClosestPtPointToTriangle(sphereCenter, triangleP0, triangleP1, triangleP2, point);

	XMVECTOR pt = XMVectorSet(point.x, point.y, point.z, 1);
	XMVECTOR center = XMVectorSet(sphereCenter.x, sphereCenter.y, sphereCenter.z, 1);

	//�_p�Ƌ��̒��S�_�̍���
	XMVECTOR v = pt - center;

	//�����̓������߂�
	v = XMVector3Dot(v, v);
	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (v.m128_f32[0] > radius * radius)
	{
		return false;
	}

	return true;
}

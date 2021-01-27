#pragma once
#include <DirectXMath.h>

//�����蔻��̌`

struct Sphere
{
	//���S���W(�Z���^�[��w��1)
	DirectX::XMVECTOR center = { 0, 0, 0, 1 };

	//���a
	float radius = 1.0f;
};

struct Plane
{
	//�@���x�N�g��(w��0)
	DirectX::XMVECTOR normal = { 0, 1, 0, 0 };

	//���_(0, 0, 0)����̋���
	float distance = 0.0f;
};

struct Line
{
	//�n�_���W
	DirectX::XMVECTOR start = { 0, 0, 0, 1 };
	//�I�_���W
	DirectX::XMVECTOR end = { 1, 0, 0, 1 };
};

//��`
struct Rectangles
{
	//���_
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float width =  10.0f;
	float height = 10.0f;
	float okuyuki = 10.0f;
	
};

//�_
struct Dot
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};
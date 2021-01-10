#pragma once
#include <cmath>

//Vector3D�N���X
class Vector3D {

public:
	//�f�t�H���g�R���X�g���N�^
	Vector3D();

	//�����t���R���X�g���N�^
	Vector3D(float x, float y, float z);
   
	//���Z�q�I�[�o�[���[�h�u+�v
	Vector3D operator+(const Vector3D& vec);

	//���Z�q�I�[�o�[���[�h�u+=�v
	Vector3D& operator+=(const Vector3D& vec);

	//���Z�q�I�[�o�[���[�h�u-�v
	Vector3D operator - (const Vector3D& vec);

	//���Z�q�I�[�o�[���[�h�u-=�v
	Vector3D& operator -= (const Vector3D& vec);

	//���Z�q�I�[�o�[���[�h�u*�v
	Vector3D operator*(const float& vec);

	//�x�N�g���̓���
	float Dot(const Vector3D& vec);

	//�x�N�g���̊O��
	float Cross(const Vector3D& vec);

	//�x�N�g���̒����擾
	float Magnitude();

	//�x�N�g���̐��K��
	Vector3D Normalized();

public:
	float x;
	float y;
	float z;
};
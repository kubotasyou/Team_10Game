#include "Vector3D.h"
//�R���X�g���N�^�R���X�g���N�^
Vector3D::Vector3D():x(0),y(0)
{}

//�����t���R���X�g���N�^
Vector3D::Vector3D(float x,float y,float z){}

//���Z�q�I�[�o�[���[�h�u+�v
Vector3D Vector3D::operator+(const Vector3D& vec)
{
	Vector3D t(this->x + vec.x, this->y + vec.y, this->z + vec.z);
	
	return t;
}

//���Z�q�I�[�o�[���[�h�u+=�v
Vector3D& Vector3D::operator+=(const Vector3D& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;

	return *this;
}

//���Z�q�I�[�o�[���[�h�u-�v
Vector3D Vector3D::operator-(const Vector3D& vec)
{
	Vector3D t(this->x - vec.x, this->y - vec.y, this->z - vec.z);

	return t;
}

//���Z�q�I�[�o�[���[�h�u-=�v
Vector3D& Vector3D::operator-=(const Vector3D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;

	return *this;
}

//���Z�q�I�[�o�[���[�h�u*�v
Vector3D Vector3D::operator*(const float & scale)
{
	Vector3D t(this->x * scale, this->y *scale, this->z*scale);

	return t;
}

//�x�N�g���̓���
float Vector3D::Dot(const Vector3D& vec)
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

//�x�N�g���̊O��
float Vector3D::Cross(const Vector3D& vec)
{
	return (y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}

//�x�N�g���̒����擾
float Vector3D::Magnitude()
{
	return static_cast<float>(sqrt(x * x + y * y + z * z));
}

//�x�N�g���̐��K��
Vector3D Vector3D::Normalized()
{
	return{ x / Magnitude(),y / Magnitude(),z / Magnitude() };
}
#include "Vector3D.h"
//コンストラクタコンストラクタ
Vector3D::Vector3D():x(0),y(0)
{}

//引数付きコンストラクタ
Vector3D::Vector3D(float x,float y,float z){}

//演算子オーバーロード「+」
Vector3D Vector3D::operator+(const Vector3D& vec)
{
	Vector3D t(this->x + vec.x, this->y + vec.y, this->z + vec.z);
	
	return t;
}

//演算子オーバーロード「+=」
Vector3D& Vector3D::operator+=(const Vector3D& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;

	return *this;
}

//演算子オーバーロード「-」
Vector3D Vector3D::operator-(const Vector3D& vec)
{
	Vector3D t(this->x - vec.x, this->y - vec.y, this->z - vec.z);

	return t;
}

//演算子オーバーロード「-=」
Vector3D& Vector3D::operator-=(const Vector3D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;

	return *this;
}

//演算子オーバーロード「*」
Vector3D Vector3D::operator*(const float & scale)
{
	Vector3D t(this->x * scale, this->y *scale, this->z*scale);

	return t;
}

//ベクトルの内積
float Vector3D::Dot(const Vector3D& vec)
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

//ベクトルの外積
float Vector3D::Cross(const Vector3D& vec)
{
	return (y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}

//ベクトルの長さ取得
float Vector3D::Magnitude()
{
	return static_cast<float>(sqrt(x * x + y * y + z * z));
}

//ベクトルの正規化
Vector3D Vector3D::Normalized()
{
	return{ x / Magnitude(),y / Magnitude(),z / Magnitude() };
}
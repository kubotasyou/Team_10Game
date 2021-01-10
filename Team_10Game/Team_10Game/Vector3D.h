#pragma once
#include <cmath>

//Vector3Dクラス
class Vector3D {

public:
	//デフォルトコンストラクタ
	Vector3D();

	//引数付きコンストラクタ
	Vector3D(float x, float y, float z);
   
	//演算子オーバーロード「+」
	Vector3D operator+(const Vector3D& vec);

	//演算子オーバーロード「+=」
	Vector3D& operator+=(const Vector3D& vec);

	//演算子オーバーロード「-」
	Vector3D operator - (const Vector3D& vec);

	//演算子オーバーロード「-=」
	Vector3D& operator -= (const Vector3D& vec);

	//演算子オーバーロード「*」
	Vector3D operator*(const float& vec);

	//ベクトルの内積
	float Dot(const Vector3D& vec);

	//ベクトルの外積
	float Cross(const Vector3D& vec);

	//ベクトルの長さ取得
	float Magnitude();

	//ベクトルの正規化
	Vector3D Normalized();

public:
	float x;
	float y;
	float z;
};
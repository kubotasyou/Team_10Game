#pragma once
#include <list>
#include "Vector3D.h"

//	弾のクラス
class PlayerBullet
{
public:
	//	コンストラクタ
	PlayerBullet(Vector3D pos);

	PlayerBullet(Vector3D pos, float angle);

	//	デストラクタ
	~PlayerBullet();

	//	開始
	void Start();

	//	描画
	void Render();

	//	更新
	void Update();

	//	座標の取得
	Vector3D Position() {
		return _position;
	}

	//	サイズの取得
	Vector3D Size() {
		return _size;
	}

private:
	const int AnimationSpeed = 4;	//	アニメーションの速度
	const int HorizonSheet = 5;		//	横画像の枚数
	const int VerticalSheet = 4;	//	縦画像の枚数
	Vector3D _position;				//	座標
	Vector3D _size;					//	画像サイズ
	Vector3D _offset;				//	オフセット座標
	Vector3D _velocity;				//	速度
	int grp;						//	画像ID
	int _animFrameCount;					//	アニメーションのフレームカウント
};
#pragma once
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "Object3D.h"
#include "GameObject.h"
#include "Model.h"

//タイトル画面シーン

class Title
{
public:

	//コンストラクタ
	Title();

	//デストラクタ
	~Title();

	//初期化
	void Initialize(DirectXManager* dxManager, Input* input);

	//更新
	void Update();

	//描画
	void Draw();

private:

	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;

	Sprite* sprite = nullptr;
	GameObject* obj = nullptr;
	Model* mod = nullptr;
};


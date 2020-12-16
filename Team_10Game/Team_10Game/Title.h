#pragma once
#include "BaseScene.h"
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"

//タイトル画面シーン

class Title : public BaseScene
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//コンスト
	Title(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input);
	//デスト
	~Title();
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//次のシーンへ
	void NextScene();

private:
	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;
	Sprite* spriteBG = nullptr;
};


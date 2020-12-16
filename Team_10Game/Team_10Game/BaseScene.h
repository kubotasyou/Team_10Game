#pragma once
#include "ISceneChanger.h"

//既定シーンクラス：各シーンはこれを継承・オーバーライド
class BaseScene
{
protected:
	//read protectedを付けることで、継承先のクラスでこれを使うことができる。
	//各継承先で、このsceneChangerが呼ばれるから、何度も作られる心配がない！
	ISceneChanger* sceneChanger;

public:
	//コンスト(継承先のsceneChangerを代入するよ)
	BaseScene(ISceneChanger* sceneChanger)
		:sceneChanger(sceneChanger) {}

	//virtualを付けたら、継承先でオーバーライドすることができる!
	virtual ~BaseScene() {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};
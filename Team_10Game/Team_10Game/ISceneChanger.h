#pragma once

//各シーンの名前
enum SceneType
{
	SceneTitle,
	SceneEnding,
	SceneNone
};

//シーンを変更するためのインターフェース
class ISceneChanger
{
public://virtualを付けたら、継承先でオーバーライドすることができる!

	//シーン変更
	virtual void ChangeScene(SceneType nextScene) = 0;

};
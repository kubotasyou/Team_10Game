#pragma once

typedef enum
{
	IScene_Title,
	IScene_GamePlay,
	IScene_Ending,
	IScene_None,
}IScene;

//シーンを変更するためのインタフェースクラス
class ISceneChanger
{
public:
	virtual ~ISceneChanger() = 0;

	//指定したシーンに変更する
	virtual void ChangeScene(IScene nextScene) = 0;
};


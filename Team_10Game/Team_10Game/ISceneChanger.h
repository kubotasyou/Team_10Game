#pragma once

typedef enum
{
	IScene_Title,
	IScene_GamePlay,
	IScene_Ending,
	IScene_None,
}IScene;

//�V�[����ύX���邽�߂̃C���^�t�F�[�X�N���X
class ISceneChanger
{
public:
	virtual ~ISceneChanger() = 0;

	//�w�肵���V�[���ɕύX����
	virtual void ChangeScene(IScene nextScene) = 0;
};


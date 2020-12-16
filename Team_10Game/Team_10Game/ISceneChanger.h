#pragma once

//�e�V�[���̖��O
enum SceneType
{
	SceneTitle,
	SceneEnding,
	SceneNone
};

//�V�[����ύX���邽�߂̃C���^�[�t�F�[�X
class ISceneChanger
{
public://virtual��t������A�p����ŃI�[�o�[���C�h���邱�Ƃ��ł���!

	//�V�[���ύX
	virtual void ChangeScene(SceneType nextScene) = 0;

};
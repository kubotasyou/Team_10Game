#pragma once
#include "ISceneChanger.h"

//����V�[���N���X�F�e�V�[���͂�����p���E�I�[�o�[���C�h
class BaseScene
{
protected:
	//read protected��t���邱�ƂŁA�p����̃N���X�ł�����g�����Ƃ��ł���B
	//�e�p����ŁA����sceneChanger���Ă΂�邩��A���x�������S�z���Ȃ��I
	ISceneChanger* sceneChanger;

public:
	//�R���X�g(�p�����sceneChanger���������)
	BaseScene(ISceneChanger* sceneChanger)
		:sceneChanger(sceneChanger) {}

	//virtual��t������A�p����ŃI�[�o�[���C�h���邱�Ƃ��ł���!
	virtual ~BaseScene() {}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};
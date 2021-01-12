#pragma once
#include "Input.h"
#include "Sound.h"
#include "Fader.h"
#include "DirectXManager.h"
#include "BaseScene.h"
#include "GameObject.h"
#include "Score.h"

class SceneManager : public ISceneChanger
{
public:
	//�R���X�g
	SceneManager(DirectXManager * manager, Input * input, Sound* sound);
	//�f�X�g
	~SceneManager();

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�V�[���؂�ւ�(���̃V�[����)
	void ChangeScene(SceneType nextScene) override;

private:
	//memo �ϐ����̐擪��[m]��t����̂́A���̃N���X�̃����o�ϐ����Ƃ킩��₷�����邽��

	BaseScene* currentScene;//���݂̃V�[��
	SceneType nextScene;	//����̃V�[��
	DirectXManager* dxManager;
	Input* input;
	Sound* sound;
	Fader* fader;
	Score* score;
};



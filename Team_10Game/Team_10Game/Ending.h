#pragma once
#include "BaseScene.h"
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "Sound.h"
#include "Score.h"

class Ending : public BaseScene
{
public:
	//�R���X�g
	Ending(ISceneChanger* sceneChanger, DirectXManager* dxcommon, Input* input,Sound* sound, Score* score);
	//�f�X�g
	~Ending();
	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//���̃V�[����
	void NextScene();

private:
	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;
	Sprite* spriteBG = nullptr;
	Sound* sound = nullptr;
	Score* score = nullptr;

	float test = 0;
};


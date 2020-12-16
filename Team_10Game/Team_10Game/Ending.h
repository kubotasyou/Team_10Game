#pragma once
#include "BaseScene.h"
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"

class Ending : public BaseScene
{
public:
	//�R���X�g
	Ending(ISceneChanger* sceneChanger, DirectXManager* dxcommon, Input* input);
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

	float test = 0;
};


#pragma once
#include "BaseScene.h"
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "Sound.h"

//�^�C�g����ʃV�[��

class Title : public BaseScene
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//�R���X�g
	Title(ISceneChanger* sceneChanger, DirectXManager* manager, Input* input,Sound* sound);
	//�f�X�g
	~Title();
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
};


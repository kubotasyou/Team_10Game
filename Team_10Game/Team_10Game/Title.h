#pragma once
#include "DirectXManager.h"
#include "Input.h"
#include "Sprite.h"
#include "Object3D.h"
#include "GameObject.h"
#include "Model.h"

//�^�C�g����ʃV�[��

class Title
{
public:

	//�R���X�g���N�^
	Title();

	//�f�X�g���N�^
	~Title();

	//������
	void Initialize(DirectXManager* dxManager, Input* input);

	//�X�V
	void Update();

	//�`��
	void Draw();

private:

	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;

	Sprite* sprite = nullptr;
	GameObject* obj = nullptr;
	Model* mod = nullptr;
};


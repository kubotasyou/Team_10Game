#pragma once
#include "Model.h"
#include "GameObject.h"

class Pointer
{
private:
	// DirectX::���ȗ�
	using float3 = DirectX::XMFLOAT3;

public:
	Pointer(Model* model);
	~Pointer();

	void Initialize();

	void Update();

	void Draw();

	float3 Position() { return position; }

private:
	//�Ə��̈ړ�
	void Move();
private:
	GameObject* pointer = nullptr;
	Model* sphereModel = nullptr;

	float3 position;//���W
	float3 rotation;//�p�x
	float3 velocity;//�ړ���
	float3 size;//�摜�T�C�Y
};
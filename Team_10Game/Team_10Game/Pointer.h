#pragma once
#include "Model.h"
#include "GameObject.h"

class Pointer
{
private:
	// DirectX::を省略
	using float3 = DirectX::XMFLOAT3;

public:
	Pointer(Model* model);
	~Pointer();

	void Initialize();

	void Update();

	void Draw();

	float3 Position() { return position; }

private:
	//照準の移動
	void Move();
private:
	GameObject* pointer = nullptr;
	Model* sphereModel = nullptr;

	float3 position;//座標
	float3 rotation;//角度
	float3 velocity;//移動量
	float3 size;//画像サイズ
};
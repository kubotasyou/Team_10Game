#include "Pointer.h"
#include "SafeDelete.h"

Pointer::Pointer(Model* model)
{
	this->position = position;
	this->sphereModel = model;

	//ポインターの生成
	pointer = GameObject::Create();
	pointer->SetModel(sphereModel);

}

Pointer::~Pointer()
{
	safedelete(pointer);
	safedelete(sphereModel);
}

void Pointer::Initialize()
{
	velocity = { 0,0,0 };
	pointer->SetPosition(position);
	pointer->SetRotation(rotation);
	pointer->SetScale({ 0.5f,0.5f,0.5f });
}

void Pointer::Update()
{
	pointer->Update();
	Move();
}

void Pointer::Draw()
{
	pointer->Draw();
}

void Pointer::Move()
{
	//必須
	velocity = { 0,0,0 };

	//位置取得
	position = pointer->GetPosition();

	position.x += velocity.x;
	position.y += velocity.y;

	pointer->SetPosition(position);
}
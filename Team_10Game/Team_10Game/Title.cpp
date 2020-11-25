#include "Title.h"
#include "SafeDelete.h"

Title::Title()
{
}

Title::~Title()
{
	safe_delete(sprite);
}

void Title::Initialize(DirectXManager * dxManager, Input * input)
{
	this->dxManager = dxManager;
	this->input = input;

	//�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/Texture/tex1.png");

	//�I�u�W�F�N�g����
	sprite = Sprite::Create(1, { -1.0f,0.0f });
}

void Title::Update()
{

}

void Title::Draw()
{
	Sprite::BeginDraw(dxManager->GetcmdList());
	sprite->Draw();
	Sprite::EndDraw();
}

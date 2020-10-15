#include "WinApp.h"
#include "DirectXManager.h"
#include "Input.h"
#include "SafeDelete.h"

#include "Sprite.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WinApp* winApp = nullptr;
	DirectXManager* dxManager = nullptr;
	Input* input = nullptr;

	//�E�B���h�E�A�v������
	winApp = new WinApp();
	winApp->CreateGameWindow();

	//DirectX������
	dxManager = new DirectXManager();
	dxManager->Initialize(winApp);

	//Input������
	input = new Input();
	input->Initialize(winApp->GetInstance(), winApp->GetHwnd());


#pragma region �e�N�X�`��������

	Sprite::StaticInitialize(dxManager->GetDevice(), WinApp::window_width, WinApp::window_height);

	Sprite* sprite = nullptr;


	//�e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/Texture/kirby.png");

	//�I�u�W�F�N�g����
	sprite = Sprite::Create(1, { -1.0f,0.0f });

#pragma endregion


	//���C�����[�v
	while (true)
	{
		if (winApp->LoopMessage())
		{
			break;
		}

		input->Update();

		//�`��J�n��
		dxManager->BeginDraw();


		if (input->GetKeyDown(KeyCode::SPACE))
		{

		}

		Sprite::BeginDraw(dxManager->GetcmdList());
		sprite->Draw();
		Sprite::EndDraw();

		//�`��I����
		dxManager->EndDraw();
	}

	//���
	safe_delete(dxManager);
	safe_delete(input);
	safe_delete(sprite);

	//�Q�[���E�B���h�E�̔j��
	winApp->ReleaseGameWindow();
	safe_delete(winApp);

	return 0;
}
#include "Ending.h"
#include "SafeDelete.h"

Ending::Ending(ISceneChanger * sceneChanger, DirectXManager * manager, Input * input,Sound * sound)
	:BaseScene(sceneChanger),//read �x�[�X�V�[���������ō���Ă���̂���...
	dxManager(manager),
	input(input),
	sound(sound)
{
}

Ending::~Ending()
{
	safe_delete(spriteBG);
}

void Ending::Initialize()
{
	// �e�N�X�`���ǂݍ���(�e�X�g�łT��)
	Sprite::LoadTexture(6, L"Resources/Texture/kirby.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(6, { 0.0f,0.0f });
}

void Ending::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		NextScene();
	}
}

void Ending::Draw()
{
	//ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();
	//Sprite::BeginDraw(cmdList);
	// �w�i�X�v���C�g�`��
	spriteBG->Draw();

	// �X�v���C�g�`��㏈��
	//Sprite::EndDraw();
}

void Ending::NextScene()
{
	//�V�[���ύX(�ύX�������V�[�������Ă�)
	sceneChanger->ChangeScene(SceneTitle);
}

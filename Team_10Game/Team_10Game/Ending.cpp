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
	safedelete(spriteBG);
}

void Ending::Initialize()
{
	// �e�N�X�`���ǂݍ���(�e�X�g�łT��)
	Sprite::LoadTexture(6, L"Resources/Texture/kirby.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(6, { 0.0f,0.0f });
	sound->PlayLoopBGM("testBgm.mp3");
}

void Ending::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		NextScene();
	}

	sound->CheckLoop("testBgm.mp3");
}

void Ending::Draw()
{
	Sprite::BeginDraw(dxManager->GetcmdList());
	// �w�i�X�v���C�g�`��
	spriteBG->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::EndDraw();
}

void Ending::NextScene()
{
	sound->StopBGM();
	//�V�[���ύX(�ύX�������V�[�������Ă�)
	sceneChanger->ChangeScene(SceneTitle);
}

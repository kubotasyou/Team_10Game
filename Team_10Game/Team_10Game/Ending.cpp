#include "Ending.h"
#include "SafeDelete.h"

Ending::Ending(ISceneChanger* sceneChanger, DirectXManager* dxcommon, Input* input, Sound* sound, Score* score)
	:BaseScene(sceneChanger),//read �x�[�X�V�[���������ō���Ă���̂���...
	dxManager(dxcommon),
	input(input),
	sound(sound),
	score(score)
{
}

Ending::~Ending()
{
	safedelete(spriteBG);
	safedelete(curssor);
}

void Ending::Initialize()
{
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(3, { 0.0f,0.0f });
	//�J�[�\���摜����
	curssor = Sprite::Create(4, { 0.0f,0.0f });
	//sound->PlayLoopBGM("testBgm.mp3");
	sound->PlayLoop("EndingDark");
}

void Ending::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		NextScene();
	}

	//sound->CheckLoop("testBgm.mp3");
}

void Ending::Draw()
{
	Sprite::BeginDraw(dxManager->GetcmdList());
	// �w�i�X�v���C�g�`��
	spriteBG->Draw();
	score->Draw(300, 350, 8, { 1,0,0,1 });

	switch (selectCount)
	{
	case 0:
		curssor->Draw();
		curssor->SetPosition({ 50.0f,50.0f });
	}

	// �X�v���C�g�`��㏈��
	Sprite::EndDraw();
}

void Ending::NextScene()
{
	sound->Stop();
	//�V�[���ύX(�ύX�������V�[�������Ă�)
	sceneChanger->ChangeScene(SceneTitle);
}

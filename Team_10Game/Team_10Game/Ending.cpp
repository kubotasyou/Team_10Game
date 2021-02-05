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


	//�����̍Đ�
	//1�񂾂������ꍇ��PlayBGM���������ɏ��������ł悢
	//sound->PlayMP3("BGM01.mp3");

	//���[�v�Đ��̏ꍇ�APlayLoopMP3���������ɏ����A
	//�������������t�@�C���������ɓ����B
	//���̌�Update��...
	//sound->PlayLoopMP3("BGM01.mp3");

	//memo : ���g���Ă���T�E���h�͂���
	sound->PlayLoopWav("EndingDark");
}

void Ending::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE)||input->GetJoyPadTrigger(JoyPad::A))
	{
		NextScene();
	}

	//���[�v�ŗ����ꍇ�A�Đ����I������������m���Ȃ��Ƃ����Ȃ����߁A
	//Updete��CheckLoop�֐��ƁA�������ɏ����������t�@�C���������ɓ����
	//���ꂪ�����ƃ��[�v���Ă���Ȃ��̂Œ��ӁB
	//sound->CheckLoop("BGM01.mp3");
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
		curssor->SetPosition({ 50.0f,490.0f });
		if (input->GetKeyTrigger(KeyCode::RIGHT)||input->GetStick("Vertices")>0)
		{
			selectCount = 1;
		}
	/*	if (input->GetKeyTrigger(KeyCode::LEFT)||input->GetStick("Vertices")<0)
		{
			selectCount = 1;
		}*/
		break;
	case 1:
		curssor->Draw();
		curssor->SetPosition({ 800.0f,490.0f });
		//if (input->GetKeyTrigger(KeyCode::RIGHT) || input->GetStick("Vertices") > 0)
		//{
		//	selectCount = 0;
		//}
		if (input->GetKeyTrigger(KeyCode::LEFT)||input->GetStick("Vertices")<0)
		{
			selectCount = 0;
		}
		break;
	}

	// �X�v���C�g�`��㏈��
	Sprite::EndDraw();
}

void Ending::NextScene()
{
	sound->StopWav();

	//�Ō�ɃV�[�����I������Ƃ��Ɏ~�߂��������t�@�C����I�����ďI���B
	//sound->StopMP3("BGM01.mp3");

	//�V�[���ύX(�ύX�������V�[�������Ă�)
	sceneChanger->ChangeScene(SceneTitle);
	switch (selectCount)
	{
	case 0:
		sceneChanger->ChangeScene(SceneTitle);
		break;
	case 1:
		sceneChanger->ChangeScene(SceneGame);
		break;
	}
}

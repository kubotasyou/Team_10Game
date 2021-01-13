#include "Title.h"
#include "SafeDelete.h"

Title::Title(ISceneChanger* sceneChanger, DirectXManager * manager, Input * input, Sound* sound)
	:BaseScene(sceneChanger),//read �x�[�X�V�[���������ō���Ă���̂���...
	dxManager(manager),
	input(input),
	sound(sound)
{
	
}

Title::~Title()
{
	safedelete(spriteBG);
}

void Title::Initialize()
{
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(2, { 0.0f,0.0f });
	//sound->PlaySE("3MinutesCooking",0.1f);
	sound->PlayLoop("Title");
}

void Title::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE))
	{	
		sound->PlaySE("select", 0.1f);
		NextScene();
	}
}

void Title::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();
	Sprite::BeginDraw(cmdList);
	// �w�i�X�v���C�g�`��
	spriteBG->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::EndDraw();
}

void Title::NextScene()
{
	sound->Stop();
	//�V�[���ύX(�ύX�������V�[�������Ă�)
	sceneChanger->ChangeScene(SceneGame);
}


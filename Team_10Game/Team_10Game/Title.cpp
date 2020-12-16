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
	safe_delete(spriteBG);
}

void Title::Initialize()
{
	// �e�N�X�`���ǂݍ���(�e�X�g�łT��)
	Sprite::LoadTexture(5, L"Resources/Texture/tex1.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(5, { 0.0f,0.0f });
	sound->Play("3MinutesCooking");
}

void Title::Update()
{
	if (input->GetKeyTrigger(KeyCode::SPACE))
	{
		
		NextScene();
	}
}

void Title::Draw()
{
	//ID3D12GraphicsCommandList* cmdList = dxManager->GetcmdList();
	//Sprite::BeginDraw(cmdList);
	// �w�i�X�v���C�g�`��
	spriteBG->Draw();

	// �X�v���C�g�`��㏈��
	//Sprite::EndDraw();
}

void Title::NextScene()
{
	//�V�[���ύX(�ύX�������V�[�������Ă�)
	sceneChanger->ChangeScene(SceneEnding);
}


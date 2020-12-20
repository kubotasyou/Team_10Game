#include "Fader.h"
#include "SafeDelete.h"

Fader::Fader(DirectXManager * manager)
	:dxManager(manager)
{
	// �e�N�X�`���ǂݍ���(�e�X�g��10��)
	Sprite::LoadTexture(10, L"Resources/Texture/black.png");
	//�w�i�X�v���C�g����
	sprite = Sprite::Create(10, { 0,0 });

	Initialize();
}

Fader::~Fader()
{
	safe_delete(sprite);
}

void Fader::Initialize()
{
	setTime_in = 0.0f;
	setTime_out = 0.0f;
	isFade = false;
	inOut = false;
	alpha = 0.0f;
	width = height = ex = 0.0f;
}

void Fader::Update()
{
	//�t�F�[�h�C�����
	if (!inOut)
	{
		FadeIn(setTime_in);
	}
	//�t�F�[�h�A�E�g���
	if (inOut)
	{

		FadeOut(setTime_out);
	}
}

void Fader::Draw()
{
	// �w�i�X�v���C�g�`��
	Sprite::BeginDraw(dxManager->GetcmdList());
	sprite->Draw();
	Sprite::EndDraw();
}

void Fader::SetFaderType(FadeType type)
{
	fadeType = type;
	switch (fadeType)
	{
	case Normal:
		sprite->SetScale({ window_width, winddow_height });
		break;
	case ScrollX:
		sprite->SetScale({ 0,winddow_height });
		break;
	case ScrollY:
		sprite->SetScale({ window_width,winddow_height });
		break;
	case Expansion:
		sprite->SetPosition({ window_width / 2,winddow_height / 2 });
		sprite->SetAnchorPoint({ 0.5f,0.5f });
		sprite->SetScale({ 0,0 });
		break;
	default:
		break;
	}
}

void Fader::SetFadeIn(float setTime)
{
	switch (fadeType)
	{
	case Normal:
		setTime_in = 1 / (setTime * 60);
		break;
	case ScrollX:
		setTime_in = window_width / (setTime * 60);
		break;
	case ScrollY:
		setTime_in = winddow_height / (setTime * 60);
		break;
	case Expansion:
		setTime_in = window_width / (setTime * 60);
		break;
	default:
		break;
	}
	isFade = true;
}

void Fader::SetFadeOut(float setTime)
{
	switch (fadeType)
	{
	case Normal:
		setTime_out = 1 / (setTime * 60);
		break;
	case ScrollX:
		setTime_out = window_width / (setTime * 60);
		break;
	case ScrollY:
		setTime_out = winddow_height / (setTime * 60);
		break;
	case Expansion:
		setTime_out = window_width / (setTime * 60);
		break;
	default:
		break;
	}
	isFade = true;
}

bool Fader::SwitchFade(bool value)
{
	return inOut = value;
}

//�摜�������Ȃ���Ԃ���A�������Ԃɕω�����B
void Fader::FadeIn(float speed)
{
	switch (fadeType)
	{
	case Normal:
		//alpha�l��1�𒴂�����
		if (alpha >= 1.0f)
		{
			//1�ŌŒ�
			sprite->SetColor({ 1.0f, 1.0f,1.0f, 1.0f });
			isFade = false;//�t�F�[�h���I��
			return;
		}

		sprite->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

		//���l�𑝂₷
		alpha += speed;
		break;
	case ScrollX:
		if (width >= window_width)
		{
			sprite->SetScale({ window_width,winddow_height });
			isFade = false;
			return;
		}
		sprite->SetScale({ width,winddow_height });
		width += speed;
		break;
	case ScrollY:
		if (height >= winddow_height)
		{
			sprite->SetScale({ window_width,winddow_height });
			isFade = false;
			return;
		}
		sprite->SetScale({ window_width,height });
		height += speed;
		break;
	case Expansion:
		if (ex >= window_width)
		{
			sprite->SetScale({ window_width,winddow_height });
			isFade = false;
			return;
		}
		sprite->SetRotation(-ex);
		sprite->SetScale({ ex,ex });
		ex += speed;
		break;
	default:
		break;
	}
}

//�摜���������Ԃ���A�����Ȃ���Ԃɕω�����B
void Fader::FadeOut(float speed)
{
	switch (fadeType)
	{
	case Normal:
		//alpha�l��0��菬�����Ȃ�����
		if (alpha < 0)
		{
			//0�ŌŒ�
			sprite->SetColor({ 1.0f, 1.0f, 1.0f, 0 });
			isFade = false;//�t�F�[�h���I��
			return;
		}

		sprite->SetColor({ 1.0f, 1.0f, 1.0f, alpha });

		//���l�����炷
		alpha -= speed;
		break;
	case ScrollX:
		if (width < 0)
		{
			sprite->SetScale({ 0,winddow_height });
			isFade = false;
			return;
		}
		sprite->SetScale({ width,winddow_height });
		width -= speed;
		break;
	case ScrollY:
		if (height < 0)
		{
			sprite->SetScale({ window_width,0 });
			isFade = false;
			return;
		}
		sprite->SetScale({ window_width,height });
		height -= speed;
		break;
	case Expansion:
		if (ex < 0)
		{
			sprite->SetScale({ 0,winddow_height });
			isFade = false;
			return;
		}
		sprite->SetRotation(ex);
		sprite->SetScale({ ex,ex });
		ex -= speed;
		break;
	default:
		break;
	}
}

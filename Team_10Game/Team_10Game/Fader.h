#pragma once
#include "DirectXManager.h"
#include "Sprite.h"
#include "WinApp.h"

enum FadeType
{
	Normal,   //�ʏ�̃t�F�[�h
	ScrollX,  //�������ɃX�N���[��
	ScrollY,  //�c�����ɃX�N���[��
	Expansion,//�g��
};

//�t�F�[�h����
class Fader
{
public:
	//�R���X�g
	Fader(DirectXManager* manager);
	//�f�X�g
	~Fader();

	void Initialize();

	void Update();

	void Draw();

	//�t�F�[�h�̎�ނ�ݒ�
	void SetFaderType(FadeType type);
	//�t�F�[�h�C���̏���(��������)
	void SetFadeIn(float setTime);
	//�t�F�[�h�A�E�g�̏���(��������)
	void SetFadeOut(float setTime);
	//��Ԃ̐؂�ւ�(false : fadeIn, true : fadeOut)
	bool SwitchFade(bool value);
	//�t�F�[�h���Ă��邩?
	bool GetisFade() { return isFade; }

private:
	//�t�F�[�h�C��(setTime_in)
	void FadeIn(float speed);
	//�t�F�[�h�A�E�g(setTime_out)
	void FadeOut(float speed);

private:
	DirectXManager* dxManager = nullptr;
	Sprite* sprite = nullptr;

	float alpha;      //�摜�̃��l
	float setTime_in; //�t�F�[�h�ɓ��鎞��
	float setTime_out;//�t�F�[�h���I��鎞��
	bool inOut;       //true : fadeOut, false : fadeIn
	bool isFade;      //�t�F�[�h���Ă��邩�H

	FadeType fadeType;
	float width, height, ex;

	const float window_width = WinApp::window_width;  //��ʉ�
	const float winddow_height = WinApp::window_height;//��ʏc
};


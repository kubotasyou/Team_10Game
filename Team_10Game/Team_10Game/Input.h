#pragma once

#include <Windows.h>
#include <wrl.h>

// DirectInput�̃o�[�W�����w��
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

using namespace Microsoft::WRL;

enum class KeyCode
{
	W = DIK_W,
	A = DIK_A,
	S = DIK_S,
	D = DIK_D,

	Z = DIK_Z,
	X = DIK_X,

	SPACE = DIK_SPACE,

	UP = DIK_UP,
	DOWN = DIK_DOWN,
	RIGHT = DIK_RIGHT,
	LEFT = DIK_LEFT,

	//NONE = 512,
};

class Input
{
public:

	Input();

	~Input();

	//������
	void Initialize(HINSTANCE hInstance,HWND hwnd);

	//�X�V
	void Update();

	//�L�[�������Ă�����
	bool GetKeyDown(KeyCode keyNum);

	//�L�[���������u��
	bool GetKeyTrigger(KeyCode keyNum);

private:
	ComPtr<IDirectInput8> dinput;
	//�L�[�{�[�h�f�o�C�X�̐���
	ComPtr<IDirectInputDevice8> devkeybord;
	//�L�[�̓��͏��
	BYTE key[256] = {};
	//�O�t���[���̃L�[���
	BYTE keyPre[256] = {};

};


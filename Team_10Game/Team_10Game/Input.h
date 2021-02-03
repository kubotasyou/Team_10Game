#pragma once
#include <Windows.h>
#include <wrl.h>
#include <dinput.h>
#include <string> 

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

enum class JoyPad
{
	A = 0,
	B = 1,
	X = 2,
	Y = 3,
	LB = 4,
	RB,
	Back,
	Start,
	LT = 10,
	RT = 11,

	Right = -1000,
};

using namespace Microsoft::WRL;

class Input
{
public:

	Input();

	~Input();

	//������
	void Initialize(HINSTANCE hInstance, HWND hwnd);

	//�X�V
	void Update();

	//�L�[�������Ă�����
	bool GetKeyDown(KeyCode keyNum);
	//�L�[���������u��
	bool GetKeyTrigger(KeyCode keyNum);
	//�L�[�𗣂����u��
	bool GetKeyRelease(KeyCode keyNum);

	//�{�^���������Ă�����
	bool GetJoyPadDown(JoyPad padNum);
	//�{�^�����������u��
	bool GetJoyPadTrigger(JoyPad padNum);
	//�{�^���𗣂����u��
	bool GetJoyPadRelease(JoyPad padNum);
	//���X�e�B�b�N����(Vertices�EHorizontal)
	float GetStick(const std::string& str);
	//�E�X�e�B�b�N����
	float GetRightStick(const std::string& str);



private:
	////DirectInput�I�u�W�F�N�g
	//ComPtr<IDirectInput8> directInput;
	//DIRECTINPUT_VERSION version = 0x0800;
	//�L�[�{�[�h�f�o�C�X�̐���
	ComPtr<IDirectInputDevice8> keybordDevice;
	//�L�[�̓��͏��
	BYTE key[256] = {};
	//�O�t���[���̃L�[���
	BYTE keyPre[256] = {};

	//ComPtr<IDirectInputDevice8> joypadDevice;
	DIDEVCAPS            diDevCaps;//DIDEVCAPS�\����

	//�p�b�h�̓��͏��
	DIJOYSTATE joyState;
	//�O�t���[���̃{�^���̏��
	BYTE joyPre[32] = {};
	//�p�b�h���ڑ�����Ă��邩
	bool connectPad = false;
};


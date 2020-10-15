#include "Input.h"

#pragma comment(lib, "dinput8.lib")

Input::Input()
{
}

Input::~Input()
{
}

void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	//�C���v�b�g������
	result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		nullptr
	);

	//�L�[�{�[�h�f�o�C�X����
	result = dinput->CreateDevice(
		GUID_SysKeyboard,
		&devkeybord,
		NULL
	);

	//���̓f�[�^�`���̃Z�b�g
	result = devkeybord->SetDataFormat(
		&c_dfDIKeyboard
	);

	//�r�����䃌�x���̃Z�b�g
	result = devkeybord->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
}

void Input::Update()
{
	HRESULT result = S_FALSE;

	//�L�[�{�[�h����J�n
	result = devkeybord->Acquire();

	//�O��̃L�[���͏����R�s�[
	//for (int i = 0; i < 256; i++)
	//{
	//	//�z����R�s�[����
	//	keyPre[i] = key[i];
	//}
	//        ��
	memcpy(keyPre, key, sizeof(key));

	//�L�[�̓���
	result = devkeybord->GetDeviceState(sizeof(key), key);
}

bool Input::GetKeyDown(KeyCode keyNum)
{
	if (key[static_cast<BYTE>(keyNum)])
	{
		return true;
	}

	return false;
}

bool Input::GetKeyTrigger(KeyCode keyNum)
{
	//�O�񉟂���Ă��Ȃ� & ���񉟂���Ă���
	if (!keyPre[static_cast<BYTE>(keyNum)] && key[static_cast<BYTE>(keyNum)])
	{
		return true;
	}

	return false;
}

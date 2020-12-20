#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace std;

//�f�o�C�X�������Ɏ��s�����֐�(�f�o�C�X�̏��EEnumDevices�œn�����l)
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

//DirectInput�I�u�W�F�N�g
ComPtr<IDirectInput8> directInput;
ComPtr<IDirectInputDevice8> joypadDevice;
//#define DIRECTINPUT_VERSION 0x0800 // DirectInput�̃o�[�W�����w��

Input::Input()
{
}

Input::~Input()
{
}

void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	//DirectInput������
	result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr
	);



	//�L�[�{�[�h�f�o�C�X����
	result = directInput->CreateDevice(
		GUID_SysKeyboard,
		&keybordDevice,
		NULL
	);

	//���̓f�[�^�`���̃Z�b�g
	result = keybordDevice->SetDataFormat(
		&c_dfDIKeyboard
	);

	//�r�����䃌�x���̃Z�b�g
	result = keybordDevice->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);


	//--------------------------------

	//�W���C�p�b�h�f�o�C�X����
	result = directInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY//���ݐڑ�����Ă���W���C�X�e�B�b�N
	);

	connectPad = false;

	//�ڑ�����Ă��Ȃ�
	if (joypadDevice == nullptr) return;

	connectPad = true;

	//�f�[�^�̌`���Z�b�g
	result = joypadDevice->SetDataFormat(&c_dfDIJoystick);

	//�������x���̃Z�b�g
	result = joypadDevice->SetCooperativeLevel(
		hwnd,
		DISCL_EXCLUSIVE | DISCL_FOREGROUND
	);

	//�f�o�C�X�̔\�͂��擾
	result = joypadDevice->GetCapabilities(&diDevCaps);

	//�\���L�[�͈̔͂̎w��?
	result = joypadDevice->EnumObjects(
		EnumAxesCallback,
		(VOID*)hwnd,
		DIDFT_AXIS
	);

	//�f�o�C�X����f�[�^���擾�ł��邩���m�F����
	result = joypadDevice->Poll();

	if FAILED(result)
	{
		//�A�N�Z�X�����擾
		result = joypadDevice->Acquire();
		while (result == DIERR_INPUTLOST)
		{
			//�ł���܂ŌJ��Ԃ�
			result = joypadDevice->Acquire();
		}
	}
}

void Input::Update()
{
	HRESULT result = S_FALSE;

	//�L�[�{�[�h����J�n
	result = keybordDevice->Acquire();

	//�O��̃L�[���͏����R�s�[
	//for (int i = 0; i < 256; i++)
	//{
	//	//�z����R�s�[����
	//	keyPre[i] = key[i];
	//}
	//        ��
	memcpy(keyPre, key, sizeof(key));

	//�L�[�̓���
	result = keybordDevice->GetDeviceState(sizeof(key), key);

	//�ڑ�����Ă��Ȃ�������
	if (!connectPad) return;

	memcpy(joyPre, joyState.rgbButtons, sizeof(joyState.rgbButtons));

	//�Q�[���p�b�h�̓��͏��擾
	result = joypadDevice->GetDeviceState(sizeof(DIJOYSTATE), &joyState);
	if FAILED(result)
	{
		//if FAILED(joypadDevice->Acquire())
		//{
		//	while (result == DIERR_INPUTLOST)
		//	{
		//		result = joypadDevice->Acquire();
		//	}
		//}
	}
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

bool Input::GetKeyRelease(KeyCode keyNum)
{
	if (keyPre[static_cast<BYTE>(keyNum)] && !key[static_cast<BYTE>(keyNum)])
	{
		return true;
	}

	return false;
}

bool Input::GetJoyPadDown(JoyPad padNum)
{
	if (joyState.rgbButtons[static_cast<BYTE>(padNum)] & 0x88)
	{
		return true;
	}
	return false;
}

bool Input::GetJoyPadTrigger(JoyPad padNum)
{
	//�O�񉟂���Ă��Ȃ� & ���񉟂���Ă���
	if (!joyPre[static_cast<BYTE>(padNum)] && joyState.rgbButtons[static_cast<BYTE>(padNum)])
	{
		return true;
	}
	return false;
}

bool Input::GetJoyPadRelease(JoyPad padnum)
{
	if (joyPre[static_cast<BYTE>(padnum)] && !joyState.rgbButtons[static_cast<BYTE>(padnum)])
	{
		return true;
	}
	return false;
}

float Input::GetStick(const std::string & str)
{
	string chara = str;
	float result = 0.0f;

	//������
	if (chara == "Vertices")
	{
		//�ڑ�����Ă�����
		if (connectPad)
		{
			//���ɌX���Ă�����
			if (joyState.lX < 0)
			{
				result = -1.0f;
			}
			//�E�ɌX���Ă�����
			else if (joyState.lX > 0)
			{
				result = +1.0f;
			}
		}
		else
		{
			if (this->GetKeyDown(KeyCode::RIGHT))
			{
				result = +1.0f;
			}
			else if (this->GetKeyDown(KeyCode::LEFT))
			{
				result = -1.0f;
			}
		}
	}
	//�c����
	if (chara == "Horizontal")
	{
		//�ڑ�����Ă�����
		if (connectPad)
		{
			//��ɌX���Ă�����
			if (joyState.lY < 0)
			{
				result = +1.0f;
			}
			//���ɌX���Ă�����
			else if (joyState.lY > 0)
			{
				result = -1.0f;
			}
		}
		else
		{
			if (this->GetKeyDown(KeyCode::UP))
			{
				result = +1.0f;
			}
			else if (this->GetKeyDown(KeyCode::DOWN))
			{
				result = -1.0f;
			}
		}
	}

	return result;
}

//�f�o�C�X�������Ɏ��s�����֐�(�f�o�C�X�̏��EEnumDevices�œn�����l)
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	HRESULT result;

	result = directInput->CreateDevice(
		pdidInstance->guidInstance,
		&joypadDevice,
		NULL
	);

	if FAILED(result)
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{
	HRESULT result;

	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;

	result = joypadDevice->SetProperty(
		DIPROP_RANGE,
		&diprg.diph
	);

	if FAILED(result)
	{
		return DIENUM_STOP;
	}

	DWORD *pdwNumForceFeedbackAxis = (DWORD*)pContext;
	if ((pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0)
	{
		(*pdwNumForceFeedbackAxis)++;
	}

	return DIENUM_CONTINUE;
}
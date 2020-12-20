#include "Input.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

using namespace std;

//デバイス発見時に実行される関数(デバイスの情報・EnumDevicesで渡した値)
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

//DirectInputオブジェクト
ComPtr<IDirectInput8> directInput;
ComPtr<IDirectInputDevice8> joypadDevice;
//#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定

Input::Input()
{
}

Input::~Input()
{
}

void Input::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result = S_FALSE;

	//DirectInput初期化
	result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput,
		nullptr
	);



	//キーボードデバイス生成
	result = directInput->CreateDevice(
		GUID_SysKeyboard,
		&keybordDevice,
		NULL
	);

	//入力データ形式のセット
	result = keybordDevice->SetDataFormat(
		&c_dfDIKeyboard
	);

	//排他制御レベルのセット
	result = keybordDevice->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);


	//--------------------------------

	//ジョイパッドデバイス生成
	result = directInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY//現在接続されているジョイスティック
	);

	connectPad = false;

	//接続されていない
	if (joypadDevice == nullptr) return;

	connectPad = true;

	//データの形式セット
	result = joypadDevice->SetDataFormat(&c_dfDIJoystick);

	//協調レベルのセット
	result = joypadDevice->SetCooperativeLevel(
		hwnd,
		DISCL_EXCLUSIVE | DISCL_FOREGROUND
	);

	//デバイスの能力を取得
	result = joypadDevice->GetCapabilities(&diDevCaps);

	//十字キーの範囲の指定?
	result = joypadDevice->EnumObjects(
		EnumAxesCallback,
		(VOID*)hwnd,
		DIDFT_AXIS
	);

	//デバイスからデータを取得できるかを確認する
	result = joypadDevice->Poll();

	if FAILED(result)
	{
		//アクセス権を取得
		result = joypadDevice->Acquire();
		while (result == DIERR_INPUTLOST)
		{
			//できるまで繰り返す
			result = joypadDevice->Acquire();
		}
	}
}

void Input::Update()
{
	HRESULT result = S_FALSE;

	//キーボード動作開始
	result = keybordDevice->Acquire();

	//前回のキー入力情報をコピー
	//for (int i = 0; i < 256; i++)
	//{
	//	//配列をコピーする
	//	keyPre[i] = key[i];
	//}
	//        ↓
	memcpy(keyPre, key, sizeof(key));

	//キーの入力
	result = keybordDevice->GetDeviceState(sizeof(key), key);

	//接続されていなかったら
	if (!connectPad) return;

	memcpy(joyPre, joyState.rgbButtons, sizeof(joyState.rgbButtons));

	//ゲームパッドの入力情報取得
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
	//前回押されていない & 今回押されている
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
	//前回押されていない & 今回押されている
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

	//横操作
	if (chara == "Vertices")
	{
		//接続されていたら
		if (connectPad)
		{
			//左に傾いていたら
			if (joyState.lX < 0)
			{
				result = -1.0f;
			}
			//右に傾いていたら
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
	//縦操作
	if (chara == "Horizontal")
	{
		//接続されていたら
		if (connectPad)
		{
			//上に傾いていたら
			if (joyState.lY < 0)
			{
				result = +1.0f;
			}
			//下に傾いていたら
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

//デバイス発見時に実行される関数(デバイスの情報・EnumDevicesで渡した値)
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
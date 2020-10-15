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

	//インプット初期化
	result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		nullptr
	);

	//キーボードデバイス生成
	result = dinput->CreateDevice(
		GUID_SysKeyboard,
		&devkeybord,
		NULL
	);

	//入力データ形式のセット
	result = devkeybord->SetDataFormat(
		&c_dfDIKeyboard
	);

	//排他制御レベルのセット
	result = devkeybord->SetCooperativeLevel(
		hwnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
}

void Input::Update()
{
	HRESULT result = S_FALSE;

	//キーボード動作開始
	result = devkeybord->Acquire();

	//前回のキー入力情報をコピー
	//for (int i = 0; i < 256; i++)
	//{
	//	//配列をコピーする
	//	keyPre[i] = key[i];
	//}
	//        ↓
	memcpy(keyPre, key, sizeof(key));

	//キーの入力
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
	//前回押されていない & 今回押されている
	if (!keyPre[static_cast<BYTE>(keyNum)] && key[static_cast<BYTE>(keyNum)])
	{
		return true;
	}

	return false;
}

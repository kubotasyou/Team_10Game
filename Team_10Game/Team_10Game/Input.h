#pragma once

#include <Windows.h>
#include <wrl.h>

// DirectInputのバージョン指定
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

using namespace Microsoft::WRL;

//

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
};

class Input
{
public:

	Input();

	~Input();

	//初期化
	void Initialize(HINSTANCE hInstance, HWND hwnd);

	//更新
	void Update();

	//キーを押している状態
	bool GetKeyDown(KeyCode keyNum);
	//キーを押した瞬間
	bool GetKeyTrigger(KeyCode keyNum);
	//キーを離した瞬間
	bool GetKeyRelease(KeyCode keyNum);

	//ボタンを押している状態
	bool GetJoyPadDown(JoyPad padNum);
	//ボタンを押した瞬間
	bool GetJoyPadTrigger(JoyPad padNum);
	//ボタンを離した瞬間
	bool GetJoyPadRelease(JoyPad padNum);



private:
	////DirectInputオブジェクト
	//ComPtr<IDirectInput8> directInput;
	//キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keybordDevice;
	//キーの入力情報
	BYTE key[256] = {};
	//前フレームのキー情報
	BYTE keyPre[256] = {};

	//ComPtr<IDirectInputDevice8> joypadDevice;
	DIDEVCAPS            diDevCaps;//DIDEVCAPS構造体


	DIJOYSTATE joyState;

	BYTE joyPre[32] = {};
};


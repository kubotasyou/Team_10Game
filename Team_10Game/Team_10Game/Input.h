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
	//左スティック操作(Vertices・Horizontal)
	float GetStick(const std::string& str);
	//右スティック操作
	float GetRightStick(const std::string& str);



private:
	////DirectInputオブジェクト
	//ComPtr<IDirectInput8> directInput;
	//DIRECTINPUT_VERSION version = 0x0800;
	//キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keybordDevice;
	//キーの入力情報
	BYTE key[256] = {};
	//前フレームのキー情報
	BYTE keyPre[256] = {};

	//ComPtr<IDirectInputDevice8> joypadDevice;
	DIDEVCAPS            diDevCaps;//DIDEVCAPS構造体

	//パッドの入力情報
	DIJOYSTATE joyState;
	//前フレームのボタンの情報
	BYTE joyPre[32] = {};
	//パッドが接続されているか
	bool connectPad = false;
};


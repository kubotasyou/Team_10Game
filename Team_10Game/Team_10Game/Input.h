#pragma once

#include <Windows.h>
#include <wrl.h>

// DirectInputのバージョン指定
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

	//初期化
	void Initialize(HINSTANCE hInstance,HWND hwnd);

	//更新
	void Update();

	//キーを押している状態
	bool GetKeyDown(KeyCode keyNum);

	//キーを押した瞬間
	bool GetKeyTrigger(KeyCode keyNum);

private:
	ComPtr<IDirectInput8> dinput;
	//キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> devkeybord;
	//キーの入力情報
	BYTE key[256] = {};
	//前フレームのキー情報
	BYTE keyPre[256] = {};

};


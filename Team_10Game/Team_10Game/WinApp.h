#pragma once

#include <Windows.h>

class WinApp
{
public://static
	//ウィンドウサイズ
	static const int window_width = 1280;
	static const int window_height = 720;

public://static
	//ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, 
		WPARAM wparam, LPARAM lparam);

public:
	//ウィンドウ作成
	void CreateGameWindow();

	//ウィンドウの登録解除
	void ReleaseGameWindow();

	//メインループのメッセージ
	bool LoopMessage();

	//ウィンドウハンドルの取得
	HWND GetHwnd()
	{
		return hwnd;
	}

	//ウィンドウインスタンスの取得
	HINSTANCE GetInstance()
	{
		return wndClass.hInstance;
	}

private:
	HWND hwnd = nullptr;  //ウィンドウハンドルの設定
	WNDCLASSEX wndClass{};//ウィンドウクラスの設定
};


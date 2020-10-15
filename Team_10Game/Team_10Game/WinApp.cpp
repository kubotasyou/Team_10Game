#include "WinApp.h"

//ウィンドウアプリ

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄された
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	//標準の初期化を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::CreateGameWindow()
{
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc;                //ウィンドウプロシージャの設定
	wndClass.lpszClassName = L"DirectXGame";//ウィンドウ名
	wndClass.hInstance = GetModuleHandle(nullptr);             //ウィンドウハンドル
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);            //カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&wndClass);

	//ウィンドウサイズ(X座標, Y座標, 横幅, 縦幅)
	RECT windowRect = { 0,0,window_width,window_height };
	//自動でサイズを補正する
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(
		wndClass.lpszClassName,            //クラス名
		L"くぁｗせｄｒｆｔｇｙふじこｌｐ；＠：「」", //タイトルバーの文字
		WS_OVERLAPPEDWINDOW,               //タイトルバーと画面に境界があるウィンドウ
		CW_USEDEFAULT,                     //表示X座標(OSに任せる)
		CW_USEDEFAULT,                     //表示Y座標(OSに任せる)
		windowRect.right - windowRect.left,//ウィンド横幅
		windowRect.bottom - windowRect.top,//ウィンド縦幅
		nullptr,                           //親のウィンドウハンドル
		nullptr,                           //メニューハンドル
		wndClass.hInstance,                //呼び出しアプリケーションハンドル
		nullptr,                           //追加パラメーター
		);

	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::ReleaseGameWindow()
{
	//ウィンドウクラスの登録を解除
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

bool WinApp::LoopMessage()
{
	//メッセージ
	MSG msg{};

	//メッセージがあるか?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//キー入力メッセージの処理
		TranslateMessage(&msg);
		//プロシージャにメッセージを送る
		DispatchMessage(&msg);
	}

	//終了メッセージが来たらループを抜ける
	//(ここにescapeで終了の処理を加えたらいいのかな？)
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

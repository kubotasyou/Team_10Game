#include "WinApp.h"

//�E�B���h�E�A�v��

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}
	//�W���̏��������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::CreateGameWindow()
{
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProc;                //�E�B���h�E�v���V�[�W���̐ݒ�
	wndClass.lpszClassName = L"DirectXGame";//�E�B���h�E��
	wndClass.hInstance = GetModuleHandle(nullptr);             //�E�B���h�E�n���h��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);            //�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&wndClass);

	//�E�B���h�E�T�C�Y(X���W, Y���W, ����, �c��)
	RECT windowRect = { 0,0,window_width,window_height };
	//�����ŃT�C�Y��␳����
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(
		wndClass.lpszClassName,            //�N���X��
		L"���������������������ӂ��������G���F�u�v", //�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,               //�^�C�g���o�[�Ɖ�ʂɋ��E������E�B���h�E
		CW_USEDEFAULT,                     //�\��X���W(OS�ɔC����)
		CW_USEDEFAULT,                     //�\��Y���W(OS�ɔC����)
		windowRect.right - windowRect.left,//�E�B���h����
		windowRect.bottom - windowRect.top,//�E�B���h�c��
		nullptr,                           //�e�̃E�B���h�E�n���h��
		nullptr,                           //���j���[�n���h��
		wndClass.hInstance,                //�Ăяo���A�v���P�[�V�����n���h��
		nullptr,                           //�ǉ��p�����[�^�[
		);

	//�E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::ReleaseGameWindow()
{
	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

bool WinApp::LoopMessage()
{
	//���b�Z�[�W
	MSG msg{};

	//���b�Z�[�W�����邩?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		//�L�[���̓��b�Z�[�W�̏���
		TranslateMessage(&msg);
		//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		DispatchMessage(&msg);
	}

	//�I�����b�Z�[�W�������烋�[�v�𔲂���
	//(������escape�ŏI���̏������������炢���̂��ȁH)
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}

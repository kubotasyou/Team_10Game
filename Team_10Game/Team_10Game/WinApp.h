#pragma once

#include <Windows.h>

class WinApp
{
public://static
	//�E�B���h�E�T�C�Y
	static const int window_width = 1280;
	static const int window_height = 720;

public://static
	//�E�B���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, 
		WPARAM wparam, LPARAM lparam);

public:
	//�E�B���h�E�쐬
	void CreateGameWindow();

	//�E�B���h�E�̓o�^����
	void ReleaseGameWindow();

	//���C�����[�v�̃��b�Z�[�W
	bool LoopMessage();

	//�E�B���h�E�n���h���̎擾
	HWND GetHwnd()
	{
		return hwnd;
	}

	//�E�B���h�E�C���X�^���X�̎擾
	HINSTANCE GetInstance()
	{
		return wndClass.hInstance;
	}

private:
	HWND hwnd = nullptr;  //�E�B���h�E�n���h���̐ݒ�
	WNDCLASSEX wndClass{};//�E�B���h�E�N���X�̐ݒ�
};


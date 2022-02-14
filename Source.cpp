//===== �C���N���[�h�� =====
#include <Windows.h>
#include "Game.h"
#include "Source.h"
#include <stdio.h>
#include "resource.h"

#pragma comment(lib,"winmm.lib")		// timeGetTime()�p

//===== �萔��` =====
#define _CRT_SECURE_NO_WARNINGS 

//===== �v���g�^�C�v�錾 =====
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
);



// �G���g���|�C���g
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	//===== �E�B���h�E�쐬�̗��� =====
	/*
	�@ �E�B���h�E�N���X���̐ݒ�
	-�E�B���h�E�����O�ɁAOS�ɍ����炱��ȃE�B���h�E���܂���I
	�Ƃ����̂������Ă���

	�A �E�B���h�E�̍쐬
	-�o�^�����������ɃE�B���h�E���쐬

	�B �E�B���h�E�̕\��
	-�쐬�����ł͉�ʂɕ\������Ȃ�
	�ʂɕ\�����Ă�����
	*/

	//----- �ϐ��錾 -----
	WNDCLASSEX wcex;	// �E�B���h�E�N���X����ݒ肷��\����
	HWND hWnd;			// �쐬�����E�B���h�E�̃n���h��
	MSG message;

	// �@ �E�B���h�E�N���X���̐ݒ�
	ZeroMemory(&wcex, sizeof(wcex));	// �\���̂̊e�����o��0������
	wcex.hInstance = hInstance;			// ���ꂩ����E�B���h�E�ƃA�v���P�[�V������R�Â�
	wcex.lpszClassName = "Class Name";	// �E�B���h�E�N���X�������ʂ��閼�O
	wcex.lpfnWndProc = WndProc;			// �E�B���h�E�v���V�[�W���̎w��
	wcex.style = CS_CLASSDC;			// �E�B���h�E�N���X�̃X�^�C��(����)���w��
	wcex.cbSize = sizeof(WNDCLASSEX);	// �\���̂̃T�C�Y���w��
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));	// �^�X�N�o�[�ɕ\������A�C�R�����w��
	wcex.hIconSm = wcex.hIcon;			// 16 * 16 �T�C�Y�̃A�C�R��
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);		// �J�[�\��
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// �w�i�̐F�w��

																	// �E�B���h�E�N���X���̓o�^
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "�E�B���h�E�N���X���̓o�^�Ɏ��s���܂���", "�G���[", MB_OK || MB_ICONERROR);
		return 0;
	}

	// �A �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,		// �ǉ��ŃE�B���h�E�̃X�^�C��(����)���w��
		wcex.lpszClassName,			// �E�B���h�E���쐬���邽�߂̃E�B���h�E�N���X���w��
		"�}���b�g�X�}�b�V���[",		// �L���v�V���������ɕ\�����閼�O
		WS_CAPTION | WS_SYSMENU,	// ��{�̃E�B���h�E�X�^�C����ݒ�
		CW_USEDEFAULT,				// �E�B���h�E�̕\���ʒu X
		CW_USEDEFAULT,				// �E�B���h�E�̕\���ʒu Y
		SCREEN_WIDTH,				// �E�B���h�E�̉���
		SCREEN_HEIGHT,				// �E�B���h�E�̏c��
		HWND_DESKTOP,				// �e�E�B���h�E�̎w��
		NULL,						// ���j���[�̎w��
		hInstance,					// �E�B���h�E��R�Â��Ă���A�v���P�[�V����
		NULL						// �s�v(�̖̂��c)
	);

	// �B �E�B���h�E�̕\��
	ShowWindow(
		hWnd,			// �\������E�B���h�E�̎w��
		nCmdShow		// �\�����@
	);
	UpdateWindow(hWnd);	// �E�B���h�E�̒��g�����t���b�V��

	// ����������
	Init(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

	// ���Ԍv���̐��x���w��
	timeBeginPeriod(1);			// 1�~���b�Ԋu

	//----- FPS����̏����� -----
	int fpsCount = 0;		// 1�b�Ԃ̏�����
	DWORD countStartTime = timeGetTime();	// �v���̊J�n����
	DWORD preExexTime = countStartTime;		// �O��̎��s����




	//----- �E�B���h�E�̊Ǘ� -----
	while (1)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))		// OS�Ŕ����������b�Z�[�W�����̃v���O�����̃L���[�Ɏ󂯎��
		{
			if (!GetMessage(&message, NULL, 0, 0))				// �L���[�Ɋi�[���ꂽ���b�Z�[�W�����o��
			{
				break;
			}
			else
			{
				TranslateMessage(&message);			// �󂯎�������b�Z�[�W���E�B���h�E�v���V�[�W���ɑ��M
				DispatchMessage(&message);
			}
		}
		else
		{
			// FPS�̐���
			DWORD nowTime = timeGetTime();		// ���ݎ���
			if (nowTime - countStartTime >= 1000)
			{
#ifdef _DEBUG
				// �^�C�g��������FPS��\��
				char fpsText[20];
				sprintf(fpsText, "FPS:%d", fpsCount);
				SetWindowText(hWnd, fpsText);
#endif // _DEBUG
				// ����1�b�Ԃ̌v������
				countStartTime = nowTime;
				fpsCount = 0;
			}
			// �Q�[���̏���
			// 60FPS(16�~���b)�o�߂���܂ł͏������Ȃ�
			if (nowTime - preExexTime >= 1000 / 60)
			{
				Update();
				Draw();

				fpsCount++;		// �Q�[���̏��������s��ꂽ
				preExexTime = nowTime;
			}
		}
	}


	//----- �I������ -----
	timeEndPeriod(1);		// ���Ԍv���̐��x��߂�
	Uninit();
	UnregisterClass(wcex.lpszClassName, hInstance);		// �E�B���h�E�N���X���̓o�^����

	return 0;
}


// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	// �󂯎�������b�Z�[�W���ɏ���
	switch (message)
	{
	case WM_DESTROY:			// �E�B���h�E������ꂽ�Ƃ�
		PostQuitMessage(0);		// WM_QUIT���b�Z�[�W�̔��s �v���O�������S�I�������閽��
		break;


	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;

		default:
			break;
		}

		break;


	default:
		break;
	}


	// �ق��̃��b�Z�[�W�͊�{�̏������s�킹��
	return DefWindowProc(
		hWnd, message,
		wParam, lParam
	);
}
//===== �C���N���[�h�� =====
#include "JoyPad.h"
#include <Windows.h>

//===== �O���[�o���ϐ� =====
XINPUT_STATE g_JoyPadState[MAX_JOYPAD];


HRESULT JoyPad::Init()
{
	ZeroMemory(g_JoyPadState, sizeof(XINPUT_STATE));		// �[��������
	
	DWORD dwResult;

	// �W���C�p�b�h�̏��i�[���ݒ�
	for (int i = 0; i < MAX_JOYPAD; i++)
	{
		dwResult = XInputGetState(i, &g_JoyPadState[i]);
		if (dwResult == ERROR_SUCCESS)
		{
			// �Ȃ�����
		}
		else
		{
			// �Ȃ����ĂȂ�
			return S_FALSE;
		}

	}

	return S_OK;
}


void JoyPad::Update()
{
	DWORD dwResult;

	// �W���C�p�b�h�̏�Ԃ��X�V
	for (int i = 0; i < MAX_JOYPAD; i++)
	{
		dwResult = XInputGetState(i, &g_JoyPadState[i]);
		if (dwResult == ERROR_SUCCESS)
		{
			// �Ȃ�����
		}
		else
		{
			// �Ȃ����ĂȂ�
		}
	}
}


// �W���C�p�b�h�̏�Ԃ��擾
XINPUT_STATE GetGamePadState(DWORD dwUserIndex)
{
	return g_JoyPadState[dwUserIndex];
}
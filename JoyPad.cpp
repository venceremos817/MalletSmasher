//===== インクルード部 =====
#include "JoyPad.h"
#include <Windows.h>

//===== グローバル変数 =====
XINPUT_STATE g_JoyPadState[MAX_JOYPAD];


HRESULT JoyPad::Init()
{
	ZeroMemory(g_JoyPadState, sizeof(XINPUT_STATE));		// ゼロ初期化
	
	DWORD dwResult;

	// ジョイパッドの情報格納先を設定
	for (int i = 0; i < MAX_JOYPAD; i++)
	{
		dwResult = XInputGetState(i, &g_JoyPadState[i]);
		if (dwResult == ERROR_SUCCESS)
		{
			// つながった
		}
		else
		{
			// つながってない
			return S_FALSE;
		}

	}

	return S_OK;
}


void JoyPad::Update()
{
	DWORD dwResult;

	// ジョイパッドの状態を更新
	for (int i = 0; i < MAX_JOYPAD; i++)
	{
		dwResult = XInputGetState(i, &g_JoyPadState[i]);
		if (dwResult == ERROR_SUCCESS)
		{
			// つながった
		}
		else
		{
			// つながってない
		}
	}
}


// ジョイパッドの状態を取得
XINPUT_STATE GetGamePadState(DWORD dwUserIndex)
{
	return g_JoyPadState[dwUserIndex];
}
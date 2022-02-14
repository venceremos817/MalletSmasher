#ifndef __JOY_PAD_H__
#define __JOY_PAD_H__

//===== インクルード部 =====
#include "DirectX.h"
#pragma comment(lib,"xinput.lib")
#include <Xinput.h>


//===== 定数定義 =====
#define MAX_JOYPAD	(2)


class JoyPad
{
public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

};

	XINPUT_STATE GetGamePadState(DWORD dwUserIndex);



#endif // !__JOY_PAD_H__

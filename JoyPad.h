#ifndef __JOY_PAD_H__
#define __JOY_PAD_H__

//===== �C���N���[�h�� =====
#include "DirectX.h"
#pragma comment(lib,"xinput.lib")
#include <Xinput.h>


//===== �萔��` =====
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

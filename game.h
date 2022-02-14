#ifndef __GAME_H__
#define __GAME_H__

#include "DirectX.h"
#include "SceneManager.h"

// �v���g�^�C�v�錾
SceneManager *GetScene();

HRESULT Init(HWND hWnd, UINT widht, UINT height);
void Uninit();
void Update();
void Draw();


#endif // !__GAME_H__


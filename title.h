#ifndef __TITLE_H__
#define __TITLE_H__

#include "DirectX.h"
#include "Stage.h"


HRESULT InitTitle();
void UninitTitle();
void UpdateTitle();
void DrawTitle();
int GetMaxScore();
StageKind GetSelectStage();


#endif // !__PLAYER_H__

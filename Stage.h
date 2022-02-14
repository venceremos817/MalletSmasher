#ifndef __STAGE_H__
#define __STAGE_H__
#include"Texture.h"


enum StageKind
{
	NormalStage,
	TopUnderPairStage,
	NormalOnlyStage,
	ConveyorStage,

	MaxStage,
	TutorialStage
};

HRESULT InitStage(StageKind);
void UninitStage();
void UpdateStage();
void DrawStage();


#endif // !__STAGE_H__


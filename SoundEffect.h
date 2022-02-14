#pragma once
//======================================
//	サウンドファイル多重読み込み防止
//======================================
#include "Sound.h"


enum SE_KIND
{
	SE_HIT_MALLET_AND_PUCK,			// 通常ヒット
	SE_SMASHHIT_MALLET_AND_PUCK,	// スマッシュヒット
	SE_CHARGE1,						// チャージ1
	SE_CHARGE2,						// チャージ2
	SE_CHARGE3,						// チャージ3

	SE_HIT_PUCK_PUCK,				// パックとパックのヒット音

	SE_HIT_WALL_PUCK,				// パックと壁のヒット音

	SE_MAX
};

enum BGM_KIND
{
	BGM_TUTORIAL_STAGE,
	BGM_GAME_STAGE,

	BGM_MAX
};

void InitSoundEffect();
void InitBGM();
void UninitSoundEffect();
void UninitBGM();

XAUDIO2_BUFFER* GetSoundEffect(SE_KIND);
XAUDIO2_BUFFER* GetBGM(BGM_KIND);
//======================================
//	サウンドファイル多重読み込み防止
//======================================

#include "SoundEffect.h"


XAUDIO2_BUFFER** g_ppSoundEffect;
XAUDIO2_BUFFER** g_ppBGM;

const char* g_pSE_FileName[SE_MAX] =
{
	"SE/PackToPack.wav",	// 通常ヒット
	"SE/SmashHit.wav",		// スマッシュヒット
	"SE/VOLUME_1.wav",		// チャージ1
	"SE/VOLUME_2.wav",		// チャージ2
	"SE/VOLUME_3.wav",		// チャージ3

	"SE/PackToWall(2).wav",			// パックとパックのヒット音
	"SE/PackToWall(3).wav"			// パックと壁のヒット音
};

const char* g_pBGM_FileName[BGM_MAX] =
{
	"Sound/title.wav",
	"Sound/Game.wav"
};



void InitSoundEffect()
{
	g_ppSoundEffect = new XAUDIO2_BUFFER*[SE_MAX];
	for (int i = 0; i < SE_MAX; i++)
	{
		g_ppSoundEffect[i] = CreateSound(g_pSE_FileName[i], false);
	}
}


void InitBGM()
{
	g_ppBGM = new XAUDIO2_BUFFER*[BGM_MAX];
	for (int i = 0; i < BGM_MAX; i++)
	{
		g_ppBGM[i] = CreateSound(g_pBGM_FileName[i], true);
	}
}


void UninitSoundEffect()
{
	delete[] g_ppSoundEffect;
}


void UninitBGM()
{
	delete[] g_ppBGM;
}


XAUDIO2_BUFFER* GetSoundEffect(SE_KIND SE_Kind)
{
	return g_ppSoundEffect[SE_Kind];
}


XAUDIO2_BUFFER* GetBGM(BGM_KIND BGM_Kind)
{
	return g_ppBGM[BGM_Kind];
}
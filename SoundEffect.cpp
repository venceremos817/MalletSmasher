//======================================
//	�T�E���h�t�@�C�����d�ǂݍ��ݖh�~
//======================================

#include "SoundEffect.h"


XAUDIO2_BUFFER** g_ppSoundEffect;
XAUDIO2_BUFFER** g_ppBGM;

const char* g_pSE_FileName[SE_MAX] =
{
	"SE/PackToPack.wav",	// �ʏ�q�b�g
	"SE/SmashHit.wav",		// �X�}�b�V���q�b�g
	"SE/VOLUME_1.wav",		// �`���[�W1
	"SE/VOLUME_2.wav",		// �`���[�W2
	"SE/VOLUME_3.wav",		// �`���[�W3

	"SE/PackToWall(2).wav",			// �p�b�N�ƃp�b�N�̃q�b�g��
	"SE/PackToWall(3).wav"			// �p�b�N�ƕǂ̃q�b�g��
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
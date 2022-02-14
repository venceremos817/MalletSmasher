#pragma once
//======================================
//	�T�E���h�t�@�C�����d�ǂݍ��ݖh�~
//======================================
#include "Sound.h"


enum SE_KIND
{
	SE_HIT_MALLET_AND_PUCK,			// �ʏ�q�b�g
	SE_SMASHHIT_MALLET_AND_PUCK,	// �X�}�b�V���q�b�g
	SE_CHARGE1,						// �`���[�W1
	SE_CHARGE2,						// �`���[�W2
	SE_CHARGE3,						// �`���[�W3

	SE_HIT_PUCK_PUCK,				// �p�b�N�ƃp�b�N�̃q�b�g��

	SE_HIT_WALL_PUCK,				// �p�b�N�ƕǂ̃q�b�g��

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
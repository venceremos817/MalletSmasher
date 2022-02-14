#pragma once

//===== �C���N���[�h�� =====
#include "ParticleEmitter.h"

class EmitterMngr
{
public:
	EmitterMngr();
	~EmitterMngr();

	void Init(int num);		// �g�������G�~�b�^�[�̐����w��
	void UnInit();
	void Update();
	void Draw();

	void DeleteEmitter();

	ParticleEmitter* GetEmitter();
	ParticleEmitter* GetFreeEmitter();

private:
	int	m_EmitterNum;
	ParticleEmitter *m_pParticleEmitter;
};


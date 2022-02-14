#pragma once

//===== インクルード部 =====
#include "ParticleEmitter.h"

class EmitterMngr
{
public:
	EmitterMngr();
	~EmitterMngr();

	void Init(int num);		// 使いたいエミッターの数を指定
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


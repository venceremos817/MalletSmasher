#include "EmitterMngr.h"



EmitterMngr::EmitterMngr()
{
}


EmitterMngr::~EmitterMngr()
{
	//m_pParticleEmitter->Uninit();
	//if (m_pParticleEmitter != nullptr)
	//{
	//	delete[] m_pParticleEmitter;
	//	m_pParticleEmitter = nullptr;
	//}
}


void EmitterMngr::Init(int num)
{
	this->m_EmitterNum = num;
	this->m_pParticleEmitter = new ParticleEmitter[num];	// エミッターを任意の数初期化
}


void EmitterMngr::UnInit()
{
	m_pParticleEmitter->Uninit();
	if (m_pParticleEmitter != nullptr)
	{
		delete[] m_pParticleEmitter;
		m_pParticleEmitter = nullptr;
	}
}


void EmitterMngr::Update()
{
	int i;
	ParticleEmitter *pEmitter = m_pParticleEmitter;

	for (i = 0; i < m_EmitterNum; i++, pEmitter++)
		pEmitter->Update();
}


void EmitterMngr::Draw()
{
	int i;
	ParticleEmitter *pEmitter = m_pParticleEmitter;

	for (i = 0; i < m_EmitterNum; i++, pEmitter++)
		pEmitter->Draw();
}


void EmitterMngr::DeleteEmitter()
{
	delete[] m_pParticleEmitter;
}


ParticleEmitter * EmitterMngr::GetEmitter()
{
	return this->m_pParticleEmitter;
}


ParticleEmitter* EmitterMngr::GetFreeEmitter()
{
	int i;
	ParticleEmitter *pEmitter = m_pParticleEmitter;

	for (i = 0; i < m_EmitterNum; i++, pEmitter++)
	{
		if(pEmitter->m_use==false)
		return pEmitter;
	}
	return nullptr;
}
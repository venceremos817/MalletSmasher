#include "ParticleEmitter.h"
#include "CMyPolygon.h"
#include <stdlib.h>
#include "DirectX.h"

//===== プロトタイプ宣言 =====

const float PI = 3.14159265359;


ParticleEmitter::ParticleEmitter()
{
	this->m_use = false;
	this->m_pParticle = nullptr;
	this->m_pTex = NULL;
	this->m_IsAddBlend = false;
}


ParticleEmitter::~ParticleEmitter()
{
	//if (m_pParticle != nullptr)
	//{
	//	delete[] this->m_pParticle;
	//	m_pParticle = nullptr;
	//}
}


void ParticleEmitter::Uninit()
{
	if (m_pParticle != nullptr)
	{
		delete[] this->m_pParticle;
		this->m_pParticle = nullptr;
	}
}


void ParticleEmitter::CreateParticle(Float2 pos, float size, Float4 color, float move, float rad, int life, int num)
{
	this->m_use = true;
	this->m_ParticleNum = num;

	this->m_pParticle = new Particle[num];
	this->m_pTex = NULL;
	this->m_IsAddBlend = false;

	int i;
	Particle *pParticle = this->m_pParticle;
	for (i = 0; i < num; i++, pParticle++)
		pParticle->SetInfo(pos, size, color, move, rad, life);
}


void ParticleEmitter::CreateParticle(int num)
{
	this->m_use = true;
	this->m_ParticleNum = num;

	this->m_pParticle = new Particle[m_ParticleNum];

	int i;
	Particle *pParticle = this->m_pParticle;
	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pParticle->SetExist(true);
	}
}


void ParticleEmitter::SetPos(Float2 pos)
{
	int i;
	Particle *pParticle = this->m_pParticle;

	for (i = 0; i < m_ParticleNum; i++,pParticle++)
	{
		pParticle->SetPos(pos);
	}
}


void ParticleEmitter::SetPosRnage(float minX, float maxX, float minY, float maxY)
{
	int i;
	Float2 pos;
	Particle *pParticle = this->m_pParticle;

	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pos(RandRange(minX, maxX), RandRange(minY, maxY));
		pParticle->SetPos(pos);
	}
}


void ParticleEmitter::SetPosRange(Float4 posRange)
{
	int i;
	Float2 pos;
	Particle *pParticle = this->m_pParticle;

	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pos(RandRange(posRange.x, posRange.y), RandRange(posRange.z, posRange.w));
		pParticle->SetPos(pos);
	}
}


void ParticleEmitter::SetSpd(float spd)
{
	int i;
	Particle *pParticle = this->m_pParticle;

	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pParticle->SetSpd(spd);
	}
}


void ParticleEmitter::SetSize(float size)
{
	int i;
	Particle *pParticle = this->m_pParticle;

	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pParticle->SetSize(size);
	}
}


void ParticleEmitter::SetColor(Float4 color)
{
	int i;
	Particle *pParticle = this->m_pParticle;

	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pParticle->SetColor(color);
	}
}


void ParticleEmitter::SetRadRange(float MinRad, float MaxRad)
{
	int i;
	Particle *pParticle = this->m_pParticle;

	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pParticle->SetRad(RandRange(MinRad, MaxRad));
	}
}


void ParticleEmitter::SetRad(float rad)
{
	int i;
	Particle *pParticle = this->m_pParticle;

	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pParticle->SetRad(rad);
	}
}


void ParticleEmitter::SetLife(int life)
{
	int i;
	Particle *pParticle = this->m_pParticle;

	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pParticle->SetLife(life);
	}
}


void ParticleEmitter::Update()
{
	if (!m_use)
		return;

	int i;
	int execution = 0;
	Particle *pParticle = this->m_pParticle;
	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		if (!pParticle->m_exist)
			continue;
		pParticle->Update();
		execution++;
	}
	if (execution == 0)
	{
		DeleteParticle();
		this->m_use = false;
		this->m_pTex = NULL;
		this->m_IsAddBlend = false;
	}
}


void ParticleEmitter::Draw()
{
	if (!m_use)
		return;

	SetPolygonTexture(this->m_pTex);
	SetAddBlend(this->m_IsAddBlend);
	int i;
	Particle *pParticle = this->m_pParticle;
	for (i = 0; i < m_ParticleNum; i++, pParticle++)
	{
		pParticle->Draw();
	}
	SetPolygonTexture(NULL);
	SetAddBlend(false);
}


void ParticleEmitter::DeleteParticle()
{
	delete[] this->m_pParticle;
	this->m_pParticle = nullptr;
}


void ParticleEmitter::SetTexture(ID3D11ShaderResourceView *pTex)
{
	this->m_pTex = pTex;
}







float RandRange(float min, float max)
{
	if (max < min)
		return 0;
	int minAngle = (int)(min * 180.0f / PI);
	int maxAngle = (int)(max * 180.0f / PI);

	float Angle = minAngle + (rand()*(maxAngle - minAngle + 1.0) / (1.0 + RAND_MAX));

	return Angle * PI / 180.0f;
}


void ParticleEmitter::IsAddBlend(bool bAddBlend)
{
	this->m_IsAddBlend = bAddBlend;
}
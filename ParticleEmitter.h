#pragma once

#include "Particle.h"
#include "Texture.h"

float RandRange(float min, float max);


class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void CreateParticle(Float2 pos, float size, Float4 color, float move, float rad, int life, int num);
	void CreateParticle(int num);
	void SetPos(Float2 pos);
	void SetPosRnage(float, float, float, float);
	void SetPosRange(Float4);
	void SetSpd(float spd);
	void SetSize(float size);
	void SetColor(Float4 color);
	void SetRadRange(float MinRad, float MaxRad);
	void SetRad(float);
	void SetLife(int life);
	void Uninit();
	void Update();
	void Draw();
	void DeleteParticle();
	void SetTexture(ID3D11ShaderResourceView*);
	void IsAddBlend(bool);

	bool	m_use;

private:
	int		m_ParticleNum;		// パーティクルの数
	Particle *m_pParticle;
	ID3D11ShaderResourceView *m_pTex;
	bool	m_IsAddBlend;
};


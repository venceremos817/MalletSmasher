#pragma once

#include "CommonClass.h"

class Particle
{
public:
	Particle();
	Particle(Float2 pos, float size, Float4 color, float move, float rad, int life);
	~Particle();

	void SetInfo(Float2 pos, float size, Float4 color, float move, float rad, int life);
	void SetPos(Float2 pos);
	void SetSize(float size);
	void SetColor(Float4 color);
	void SetSpd(float spd);
	void SetRad(float rad);
	void SetLife(int life);
	void SetExist(bool exist);
	void Update();
	void Draw();

	bool	m_exist;

private:
	Float2	m_Pos;			// �ʒu
	Float4	m_Color;		// �F
	float	m_Spd;			// ����
	float	m_rad;			// �i�s����
	int		m_ExecuteTime;	// �o�ߎ���
	float	m_Size;			// �T�C�Y
	int		m_TimeToLive;	// ��������
};


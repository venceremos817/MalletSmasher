#include "Particle.h"
#include <math.h>
#include "CMyPolygon.h"
#include <random>
#include "DirectX.h"

#define PI	(3.14159265359)


Particle::Particle()
{

}

Particle::Particle(Float2 pos, float size, Float4 color,float move,float rad,int life)
{
	this->m_Pos = pos;
	this->m_Size = size;
	this->m_Color = color;
	this->m_Spd = move;
	this->m_rad = rad;
	//this->m_rad = rand()*PI / 180.0f;
	this->m_TimeToLive = life;
	this->m_ExecuteTime = 0;
	this->m_exist = true;
}


Particle::~Particle()
{
}


void Particle::SetInfo(Float2 pos, float size, Float4 color, float move, float rad, int life)
{
	this->m_Pos = pos;
	this->m_Size = size;
	this->m_Color = color;
	this->m_Spd = move;
	this->m_rad = rad;
	//this->m_rad = rand()*PI / 180.0f;
	this->m_TimeToLive = life;
	this->m_ExecuteTime = 0;
	this->m_exist = true;
}


void Particle::SetPos(Float2 pos)
{
	this->m_Pos = pos;
}


void Particle::SetSize(float size)
{
	this->m_Size = size;
}


void Particle::SetColor(Float4 color)
{
	this->m_Color = color;
}


void Particle::SetSpd(float spd)
{
	this->m_Spd = spd;
}


void Particle::SetRad(float rad)
{
	this->m_rad = rad;
}


void Particle::SetLife(int life)
{
	this->m_TimeToLive = life;
	this->m_ExecuteTime = 0;
}


void Particle::SetExist(bool exist)
{
	this->m_exist = exist;
}


void Particle::Update()
{
	if (!m_exist)
		return;
	m_Pos.x += m_Spd * cosf(m_rad);
	m_Pos.y += m_Spd * sinf(m_rad);
	m_ExecuteTime++;
	if (m_TimeToLive < m_ExecuteTime)
		m_exist = false;
}


void Particle::Draw()
{
	if (!m_exist)
		return;
	CMyPolygon polygon;

	polygon.SetPos(m_Pos.x, m_Pos.y);
	polygon.SetSize(m_Size, m_Size);
	float alpha = m_Color.w - ((float)m_ExecuteTime / (float)m_TimeToLive);
	polygon.SetColor(m_Color.x, m_Color.y, m_Color.z, alpha);
	polygon.Draw();
}

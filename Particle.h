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
	Float2	m_Pos;			// 位置
	Float4	m_Color;		// 色
	float	m_Spd;			// 速さ
	float	m_rad;			// 進行方向
	int		m_ExecuteTime;	// 経過時間
	float	m_Size;			// サイズ
	int		m_TimeToLive;	// 生存時間
};


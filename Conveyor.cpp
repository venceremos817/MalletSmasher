#include "Conveyor.h"
#include "myPolygon.h"
#include "Collision.h"
#include "Puck.h"
#include <math.h>
#include "CMyPolygon.h"

#define PI (3.14159265359)


HRESULT Conveyor:: Init()
{

	HRESULT hr = S_OK;
	hr = LoadTextureFromFile("image/Circle.png", &m_pTex);
	if (FAILED(hr))
	{
		return hr;
	}

	m_PosX = 0.0f;
	m_PosY = 0.0f;
	m_width = 50.0f;
	m_height = 50.0f;
	m_radian = PI / 2.0f;
	m_anime = 0.0f;
	m_RotSpd = 0.0f;
	m_HitConveyorToPack = false;
	m_Delay = 0;
	m_Alpha = 1.0f;

	return hr;
}
void Conveyor:: Uninit()
{
	SAFE_RELEASE(m_pTex);
}
void Conveyor:: Update()
{
	
	// パックとの当たり判定
	PuckInfo *pPuck;
	pPuck = GetPuckInfo();
	
	for (int i = 0; i < MAX_PUCK; i++, pPuck++)
	{
		if (m_HitConveyorToPack != true)
		{
			// パックとバンパーの当たり判定
			if (CheckCollisionCircleToCircle(m_PosX, m_PosY, pPuck->CurPosx, pPuck->CurPosy, m_radian, pPuck->radius))
			{
				// 当たったかの判定をいれる
				m_HitConveyorToPack = true;
				// 強い反射
				pPuck->movex *= -3.0f;
				pPuck->movey *= -3.0f;
				// ディレイを0に設定する
				m_Delay = 0;
				// 半透明にする
				m_Alpha = 0.3f;
				Ploygon.SetAlpha(m_Alpha);
			}
		}
		else
		{
			if (CheckCollisionCircleToCircle(m_PosX, m_PosY, pPuck->CurPosx, pPuck->CurPosy, m_radian, pPuck->radius))
			{
				m_Delay++;
				if (m_Delay > 10)
				{
					// 当たったかの判定を消す
					m_HitConveyorToPack = false;
					// 半透明を元に戻す
					m_Alpha = 1.0f;
					Ploygon.SetAlpha(m_Alpha);
				}
			}
		}
		
	}
}
void Conveyor:: Draw()
{

	SetPolygonTexture(m_pTex);
	SetPolygonAngle(0.0f);

	Ploygon.SetPos(m_PosX, m_PosY);
	Ploygon.SetSize(m_width, m_height);
	Ploygon.SetColor(1.0f, 1.0f, 1.0f, m_Alpha);
	Ploygon.Draw();
}
void Conveyor:: SetPos(float PosX, float PosY)
{
	m_PosX = PosX;
	m_PosY = PosY;
}
void Conveyor::SetRotSpd(float RotSpd)
{
	m_RotSpd = RotSpd;
}
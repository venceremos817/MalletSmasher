#include "fade.h"
#include "Source.h"
#include "myPolygon.h"


HRESULT Fade:: Init()
{
	HRESULT hr = S_OK;

	m_FadeRequireTime = 0;
	m_FadeElapseTime = 0;
	m_IsOut = false;

	return hr;
}
void Fade::Uninit()
{

}
void Fade::Update()
{
	if (m_FadeElapseTime < m_FadeRequireTime)
	{
		m_FadeElapseTime += 1.0f / 60;
	}
	else
	{
		m_FadeRequireTime = 0;
	}

}
void Fade::Draw()
{
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	SetPolygonTexture(NULL);
	float alpha = 1;
	// 分母が0はダメ
	if (m_FadeRequireTime != 0)
	{
		alpha = m_FadeElapseTime / m_FadeRequireTime;
	}
	if (m_IsOut == true)
	{
		SetPolygonVtxColor(0, 0.0f, 0, 0, alpha);
		SetPolygonVtxColor(1, 0.0f, 0, 0, alpha);
		SetPolygonVtxColor(2, 0.0f, 0, 0, alpha);
		SetPolygonVtxColor(3, 0.0f, 0, 0, alpha);
	}
	else
	{
		SetPolygonVtxColor(0, 0.0f, 0, 0, 1.0f - alpha);
		SetPolygonVtxColor(1, 0.0f, 0, 0, 1.0f - alpha);
		SetPolygonVtxColor(2, 0.0f, 0, 0, 1.0f - alpha);
		SetPolygonVtxColor(3, 0.0f, 0, 0, 1.0f - alpha);
	}

	DrawPolygon();
}

// フェード開始 クラスの型名をつけろ雑魚
void Fade::Start(float RequireTime, bool Out)
{
	m_FadeRequireTime = RequireTime;
	m_IsOut = Out;

	m_FadeElapseTime = 0.0f;

}
bool Fade::IsFinsh()
{
	if (m_FadeRequireTime == 0)
	{
		return true;
	}

	return false;
	
}
bool Fade::IsExecute()
{
	if (m_FadeRequireTime != 0)
	{
		return true;
	}

	return false;

}
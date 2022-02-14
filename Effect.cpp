#include "Effect.h"
#include "Source.h"
#include "myPolygon.h"
#include <math.h>

#define MAX_EFFECT (2)
#define PI (3.14159265359)
#define MAX_COUNT (3)
#define EFFECT_RATE (20)

// ぐろばる
GoalEffect g_GoalEffect[MAX_EFFECT];



void InitEffect()
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		g_GoalEffect[i].width = SCREEN_WIDTH / 2.0f;
		g_GoalEffect[i].height = SCREEN_HEIGHT;
		g_GoalEffect[i].posy = SCREEN_HEIGHT/2.0f;
		g_GoalEffect[i].Alpha = 0.0f;
		g_GoalEffect[i].exist = false;
		g_GoalEffect[i].Count = 0;
		g_GoalEffect[i].EffectTime = 0.0f;
	}
	g_GoalEffect[0].posx = SCREEN_WIDTH / 4.0f;
	g_GoalEffect[1].posx = SCREEN_WIDTH * 3.0f / 4.0f;
	
}
void UninitEffect()
{

}
void UpdateEffect()
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_GoalEffect[i].exist == false)
		{
			continue;
		}
		// エフェクトのタイマーの更新
		g_GoalEffect[i].EffectTime += EFFECT_RATE;
		// sin波を使って0から1をだす
		g_GoalEffect[i].Alpha = fabs(cosf(g_GoalEffect[i].EffectTime*PI / 180.0f)*0.5f-0.5f);
		
		if (g_GoalEffect[i].EffectTime%360==0)
		{
			g_GoalEffect[i].Count++;
		}

		if (g_GoalEffect[i].Count >= MAX_COUNT)
		{
			g_GoalEffect[i].exist = false;
		}
	}
}
void DrawEffect()
{
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		if (g_GoalEffect[i].exist == false)
		{
			continue;
		}

		SetPolygonPos(g_GoalEffect[i].posx, g_GoalEffect[i].posy);
		SetPolygonSize(g_GoalEffect[i].width, g_GoalEffect[i].height);
		switch (i)
		{
			// 左
		case 0:

			SetPolygonVtxColor(0, 0.0f, 0.0f, 1.0f, g_GoalEffect[i].Alpha);
			SetPolygonVtxColor(1, 0.0f, 0.0f, 1.0f, 0.0f);
			SetPolygonVtxColor(2, 0.0f, 0.0f, 1.0f, g_GoalEffect[i].Alpha);
			SetPolygonVtxColor(3, 0.0f, 0.0f, 1.0f, 0.0f);

			break;
			// 右
		case 1:

			SetPolygonVtxColor(0, 1.0f, 0.0f, 0.0f, 0.0f);
			SetPolygonVtxColor(1, 1.0f, 0.0f, 0.0f, g_GoalEffect[i].Alpha);
			SetPolygonVtxColor(2, 1.0f, 0.0f, 0.0f, 0.0f);
			SetPolygonVtxColor(3, 1.0f, 0.0f, 0.0f, g_GoalEffect[i].Alpha);
			break;
		default:
			break;
		}

		DrawPolygon();

	}
}
void StartEffect(int side)
{
	g_GoalEffect[side].exist = true;
	// エフェクトの生存時間タイマーの初期化
	g_GoalEffect[side].Count = 0;
	// エフェクトのタイマーの初期化
	g_GoalEffect[side].EffectTime = 0.0f;

}
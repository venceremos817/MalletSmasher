#include "goal.h"
#include "mallet.h"
#include "Puck.h"
#include "myPolygon.h"
#include <math.h>

#define GOALWIDTH (MALLETRADIUS)
#define MAXGOAL (2)

// グローバル変数
GoalInfo g_goal[2];


void InitGoal()
{
	// ゴール左
	g_goal[0].posx = 0;
	g_goal[0].posy = SCREEN_HEIGHT / 2.0f;
	g_goal[0].width = GOALWIDTH;
	g_goal[0].height = GOALHEIGHT;
	g_goal[0].Alpha = 1.0f;
	g_goal[0].Red = 0.0f;
	g_goal[0].Blue = 1.0f;
	g_goal[0].Green = 0.0f;

	// ゴール右
	g_goal[1].posx = SCREEN_WIDTH;
	g_goal[1].posy = SCREEN_HEIGHT / 2.0f;
	g_goal[1].width = GOALWIDTH;
	g_goal[1].height = GOALHEIGHT;
	g_goal[1].Alpha = 1.0f;
	g_goal[1].Red = 1.0f;
	g_goal[1].Blue = 0.0f;
	g_goal[1].Green = 0.0f;

}
void UninitGoal()
{

}
void UpdateGoal()
{

}
void DrawGoal()
{
	SetPolygonTexture(NULL);

	for (int i = 0; i < MAXGOAL; i++)
	{

		SetPolygonPos(g_goal[i].posx, g_goal[i].posy);
		SetPolygonSize(g_goal[i].width, g_goal[i].height);
		SetPolygonVtxColor(0, g_goal[i].Red, g_goal[i].Green, g_goal[i].Blue, g_goal[i].Alpha);
		SetPolygonVtxColor(1, g_goal[i].Red, g_goal[i].Green, g_goal[i].Blue, g_goal[i].Alpha);
		SetPolygonVtxColor(2, g_goal[i].Red, g_goal[i].Green, g_goal[i].Blue, g_goal[i].Alpha);
		SetPolygonVtxColor(3, g_goal[i].Red, g_goal[i].Green, g_goal[i].Blue, g_goal[i].Alpha);

		DrawPolygon();

	}
}

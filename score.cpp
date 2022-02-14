#include "score.h"
#include "myPolygon.h"
#include "Texture.h"
#include "game.h"
#include "fade.h"
#include "Sound.h"
#include "title.h"

// 定数定義
#define MAX_SCORE_DIGIT (2)
#define ANIM_SCORE_SPLIT_X (10)  // 画像の分割数(横)
#define ANIM_SCORE_SPLIT_Y (1)  // 画像の分割数(縦)
#define ScoreWidht (80)
#define ScoreHeight (100)

// 関数
void InitScore_1P();
void UpdateScore_1P();
void DrawScore_1P();
void InitScore_2P();
void UpdateScore_2P();
void DrawScore_2P();

// ぐるばる
ID3D11ShaderResourceView *g_pScore_1PTex;
ID3D11ShaderResourceView *g_pScore_2PTex;
ScoreInfo g_Score_1P;
ScoreInfo g_Score_2P;


HRESULT InitScore()
{
	HRESULT hr = S_OK;
	

	InitScore_1P();
	InitScore_2P();

	//テクスチャの貼り付け 
	hr = LoadTextureFromFile("image/number.png", &g_pScore_1PTex);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = LoadTextureFromFile("image/number.png", &g_pScore_2PTex);

	return hr;
}
void UninitScore()
{
	SAFE_RELEASE(g_pScore_1PTex);
	SAFE_RELEASE(g_pScore_2PTex);
}
void UpdateScore()
{
	UpdateScore_1P();
	UpdateScore_2P();
}
void DrawScore()
{
	DrawScore_1P();

	DrawScore_2P();
	
}
void AddScore_1P(int score)
{
	g_Score_1P.Score += score;
}

void AddScore_2P(int score)
{
	g_Score_2P.Score += score;
}

void InitScore_1P()
{
	g_Score_1P.Score = 0;
}
void UpdateScore_1P()
{
	
	if (GetScene()->GetFade().IsExecute() == false)
	{
		if (g_Score_1P.Score > GetMaxScore())
		{
			
			GetScene()->SetNextScene(Scene_Title);
		}
	}
}
void DrawScore_1P()
{
	int work = 0;
	int ScorePos = SCREEN_WIDTH / 2 - ScoreWidht*2;
	//　今持っている点数を保存しておく
	work = g_Score_1P.Score;

	SetPolygonSize(ScoreWidht, ScoreHeight);
	SetPolygonVtxColor(0, 1, 1, 1, 1);
	SetPolygonVtxColor(1, 1, 1, 1, 1);
	SetPolygonVtxColor(2, 1, 1, 1, 1);
	SetPolygonVtxColor(3, 1, 1, 1, 1);
	SetPolygonTexture(g_pScore_1PTex);

	do
	{
		SetPolygonPos(ScorePos, 0 + ScoreHeight);
		SetPolygonSplitUV(work % 10, ANIM_SCORE_SPLIT_X, ANIM_SCORE_SPLIT_Y);
		DrawPolygon();
		work = work / 10;
		ScorePos=ScorePos-ScoreWidht;
	} while (work > 0);
	
}

void InitScore_2P()
{
	g_Score_2P.Score = 0;
}
void UpdateScore_2P()
{
	if (GetScene()->GetFade().IsExecute() == false)
	{

		if (g_Score_2P.Score > GetMaxScore())
		{
			GetScene()->SetNextScene(Scene_Title);
		}
	}
}

void DrawScore_2P()
{
	int work = 0;
	int ScorePos = SCREEN_WIDTH / 2 + ScoreWidht*2;

	work = g_Score_2P.Score;
	
	SetPolygonSize(ScoreWidht, ScoreHeight);
	SetPolygonVtxColor(0, 1, 1, 1, 1);
	SetPolygonVtxColor(1, 1, 1, 1, 1);
	SetPolygonVtxColor(2, 1, 1, 1, 1);
	SetPolygonVtxColor(3, 1, 1, 1, 1);
	SetPolygonTexture(g_pScore_2PTex);

	do
	{
		SetPolygonPos(ScorePos, 0 + ScoreHeight);
		SetPolygonSplitUV(work % 10, ANIM_SCORE_SPLIT_X, ANIM_SCORE_SPLIT_Y);
		DrawPolygon();
		work = work / 10;
		ScorePos = ScorePos - ScoreWidht;
	} while (work > 0);

}
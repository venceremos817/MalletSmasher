#include "Puck.h"
#include "Texture.h"
#include "Source.h"
#include "myPolygon.h"
#include "mallet.h"
#include <math.h>
#include "score.h"
#include <stdlib.h>
#include <time.h>
#include "Collision.h"
#include "Effect.h"
#include <stdlib.h>
#include <math.h>
#include "ParticleEmitter.h"
#include "game.h"
#include "SoundEffect.h"
#include "myVector.h"


// 定数 マクロ定義
#define BONUSSCORE (3)
#define PI (3.1415)
#define RANDOM(min,max) (min+(rand()*max-min+1.0)/(1.0+RAND_MAX))


// 関数
void CreateNormalPuck(float Posx,float Posy);
void CreateDualPuck(float Posx, float Posy);
void CreateBonusPuck(float Posx, float Posy);
void CreateMiniPuck(float Posx, float Posy);
void CreateGhostPuck(float Posx, float Posy);
void PuckColProcess(PuckInfo *a, PuckInfo *b);


// ぐろばる
ID3D11ShaderResourceView *g_pPuckTex;
ID3D11ShaderResourceView *g_pPuckAfterTex;
PuckInfo g_Puck[MAX_PUCK];
IXAudio2SourceVoice *g_pPuckSESpeaker;
int g_rad;




HRESULT InitPuck()
{
	// ランダムの初期化
	srand((unsigned int)time(NULL));

	g_rad = 0.0f;

	for (int i = 0; i < MAX_PUCK; i++)
	{
		g_Puck[i].CurPosx = SCREEN_WIDTH / 2;
		g_Puck[i].CurPosy = SCREEN_HEIGHT / 2;
		g_Puck[i].movex = 0.0f;
		g_Puck[i].movey = 0.0f;
		g_Puck[i].PrePosx = g_Puck[i].CurPosx;
		g_Puck[i].PrePosy = g_Puck[i].CurPosy;
		for (int j = 0; j < MAX_PRIMITIVE; j++)
		{
			g_Puck[i].AE[i].Pos.x = g_Puck[i].CurPosx;
			g_Puck[i].AE[i].Pos.y = g_Puck[i].CurPosy;
		}
		g_Puck[i].radius = PUCKRADIUS / 2.0f;
		g_Puck[i].mass = 2.0f;
		g_Puck[i].Alpha = 1.0f;
		g_Puck[i].color.SetRGB(1.0f, 1.0f, 0.0f);
		g_Puck[i].Kind = NormalPuck;
		g_Puck[i].Score = 1;
		g_Puck[i].exist = false;
		g_Puck[i].HitConveyor = true;
		g_Puck[i].HitStopFrame = 0;
	}
	g_Puck[0].exist = true;

	HRESULT hr;

	hr = LoadTextureFromFile("image/Circle.png", &g_pPuckAfterTex);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = LoadTextureFromFile("image/OutlineCircle.png", &g_pPuckTex);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}

void UninitPuck()
{
	// テクスチャの開放
	SAFE_RELEASE(g_pPuckTex);
	SAFE_RELEASE(g_pPuckAfterTex);
}
void UpdatePuck()
{

	for (int i = 0; i < MAX_PUCK; i++)
	{
		if (g_Puck[i].exist == false)
		{
			// forの先頭に戻る
			continue;

		}
		if (g_Puck[i].HitStopFrame > 0)
		{
			g_Puck[i].HitStopFrame--;
			continue;
		}

		// α値の設定
		g_rad += 1.0f;

		// 摩擦
		g_Puck[i].movex *= 0.999f;
		g_Puck[i].movey *= 0.999f;

		// 座標退避
		g_Puck[i].PrePosx = g_Puck[i].CurPosx;
		g_Puck[i].PrePosy = g_Puck[i].CurPosy;

		// 残像更新
		g_Puck[i].AE[0].Pos.x = g_Puck[i].PrePosx;
		g_Puck[i].AE[0].Pos.y = g_Puck[i].PrePosy;
		g_Puck[i].AE[0].radius = g_Puck[i].radius;
		g_Puck[i].AE[0].Color.x = g_Puck[i].color.GetRed();
		g_Puck[i].AE[0].Color.y = g_Puck[i].color.GetGreen();
		g_Puck[i].AE[0].Color.z = g_Puck[i].color.GetBlue();
		g_Puck[i].AE[0].Color.w = g_Puck[i].Alpha;
		for (int j = MAX_PRIMITIVE - 1; j > 0; j--)
		{
			g_Puck[i].AE[j] = g_Puck[i].AE[j - 1];
			g_Puck[i].AE[j].radius = g_Puck[i].AE[j - 1].radius;
			g_Puck[i].AE[j].Color = g_Puck[i].AE[j - 1].Color;
			g_Puck[i].AE[j].Color.w = g_Puck[i].AE[j - 1].Color.w - 1.0f / MAX_PRIMITIVE;
		}

		// 速度制限
		if (g_Puck[i].Kind != MiniPuck)
		{
			if (((g_Puck[i].movex)*(g_Puck[i].movex) + (g_Puck[i].movey)*(g_Puck[i].movey)) > (MAX_PACK_SPEED * MAX_PACK_SPEED))
			{
				g_Puck[i].movex = (g_Puck[i].movex * MAX_PACK_SPEED) / (fabsf(g_Puck[i].movex) + fabsf(g_Puck[i].movey));
				g_Puck[i].movey = (g_Puck[i].movey * MAX_PACK_SPEED) / (fabsf(g_Puck[i].movex) + fabsf(g_Puck[i].movey));
			}
		}

		// 座標更新
		g_Puck[i].CurPosx += g_Puck[i].movex;
		g_Puck[i].CurPosy += g_Puck[i].movey;


		// パック同士の当たり判定
		for (int j = i + 1; j < MAX_PUCK; j++)
		{
			if (g_Puck[j].exist == false)
				continue;
			if (CheckCollisionCircleToCircle(g_Puck[i].CurPosx, g_Puck[i].CurPosy, g_Puck[j].CurPosx, g_Puck[j].CurPosy, g_Puck[i].radius, g_Puck[j].radius))
			{
				myVector2 vec1(g_Puck[i].PrePosx - g_Puck[i].CurPosx, g_Puck[i].PrePosy - g_Puck[i].CurPosy);
				myVector2 vec2(g_Puck[j].PrePosx - g_Puck[j].CurPosx, g_Puck[j].PrePosy - g_Puck[j].CurPosy);
				myVector2 vec3 = vec2 - vec1;
				float rad = atan2f(g_Puck[j].PrePosy - g_Puck[i].PrePosy, g_Puck[j].PrePosx - g_Puck[i].PrePosx);
				float SeVol = vec3.x*cosf(rad) + vec3.y * sinf(rad);
				g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_HIT_PUCK_PUCK));
				g_pPuckSESpeaker->SetVolume(SeVol / MALLET_SPEED);

				PuckColProcess(&g_Puck[i], &g_Puck[j]);
			}

		}

		// ゴーストパックのα値設定
		if (g_Puck[i].Kind == GhostPuck)
		{
			float rad = 0.0f;
			rad = sinf(g_rad*2.0f / 180 * PI)*0.5f + 0.5f;
			g_Puck[i].Alpha = rad;
		}


		if (g_Puck[i].Kind == BonusPuck)
		{
			// パックからパーティクル出す
			// 軌跡
			static int particlecnt = 0;
			particlecnt++;
			particlecnt %= 2;
			//if (particlecnt == 0)
			{
				ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(RandRange(g_Puck[i].CurPosx - g_Puck[i].radius, g_Puck[i].CurPosx + g_Puck[i].radius), RandRange(g_Puck[i].CurPosy - g_Puck[i].radius, g_Puck[i].CurPosy + g_Puck[i].radius));
					Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(1);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(30);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(5);
					pEmitter->SetSpd(0);
					pEmitter->SetRadRange(0, 2.0f * PI);
					//pEmitter->SetTexture(g_pPuckTex);
					pEmitter->IsAddBlend(true);
				}
			}

			// ゲーミングパック
			static float hue = 0.0f;
			g_Puck[i].color.SetHue(hue);
			hue += 0.01f;
			while (hue > 1.0f)
			{
				hue -= 1.0f;
			}
		}
	}

}
void DrawPuck()
{

	SetPolygonSplitUV(0, 1, 1);

	for (int i = 0; i < MAX_PUCK; i++)
	{

		if (g_Puck[i].exist == false)
		{
			continue;
		}

		// 残像描画
		int AENum = sqrt(g_Puck[i].movex*g_Puck[i].movex + g_Puck[i].movey*g_Puck[i].movey) / 5;
		SetPolygonSize(g_Puck[i].radius*1.8f, g_Puck[i].radius*1.8f);
		SetPolygonTexture(g_pPuckAfterTex);
		SetAddBlend(true);
		for (int j = 0; j < MAX_PRIMITIVE && AENum > 0; j++, AENum--)
		{
			SetPolygonVtxColor(0, g_Puck[i].AE[j].Color.x, g_Puck[i].AE[j].Color.y, g_Puck[i].AE[j].Color.z, g_Puck[i].AE[j].Color.w);
			SetPolygonVtxColor(1, g_Puck[i].AE[j].Color.x, g_Puck[i].AE[j].Color.y, g_Puck[i].AE[j].Color.z, g_Puck[i].AE[j].Color.w);
			SetPolygonVtxColor(2, g_Puck[i].AE[j].Color.x, g_Puck[i].AE[j].Color.y, g_Puck[i].AE[j].Color.z, g_Puck[i].AE[j].Color.w);
			SetPolygonVtxColor(3, g_Puck[i].AE[j].Color.x, g_Puck[i].AE[j].Color.y, g_Puck[i].AE[j].Color.z, g_Puck[i].AE[j].Color.w);
			SetPolygonPos(g_Puck[i].AE[j].Pos.x, g_Puck[i].AE[j].Pos.y);
			SetPolygonSize(g_Puck[i].AE[j].radius * 2, g_Puck[i].AE[j].radius * 2);
			DrawPolygon();
		}
		SetAddBlend(false);



		// パック描画
		SetPolygonTexture(g_pPuckAfterTex);

		SetPolygonVtxColor(0, g_Puck[i].color.GetRed(), g_Puck[i].color.GetGreen(), g_Puck[i].color.GetBlue(), g_Puck[i].Alpha);
		SetPolygonVtxColor(1, g_Puck[i].color.GetRed(), g_Puck[i].color.GetGreen(), g_Puck[i].color.GetBlue(), g_Puck[i].Alpha);
		SetPolygonVtxColor(2, g_Puck[i].color.GetRed(), g_Puck[i].color.GetGreen(), g_Puck[i].color.GetBlue(), g_Puck[i].Alpha);
		SetPolygonVtxColor(3, g_Puck[i].color.GetRed(), g_Puck[i].color.GetGreen(), g_Puck[i].color.GetBlue(), g_Puck[i].Alpha);

		SetPolygonPos(g_Puck[i].CurPosx, g_Puck[i].CurPosy);
		SetPolygonSize(g_Puck[i].radius*2.0f, g_Puck[i].radius*2.0f);

		SetPolygonAngle(0.0f);

		// 描画
		DrawPolygon();

	}
}

// パック情報取得
PuckInfo *GetPuckInfo()
{
	return g_Puck;
}
// パック生成
void CreatePuck(PuckKind kind,float Posx,float Posy)
{

	for (int i = 0; i < MAX_PUCK; i++)
	{
		g_Puck[i].exist = false;
	}

#ifdef _DEBUG
	static int cnt = 0;
	switch ((cnt++) % MaxPuckKind)
	{
	case NormalPuck:

		CreateNormalPuck(Posx, Posy);

		break;
	case DualPuck:

		CreateDualPuck(Posx, Posy);

		break;
	case BonusPuck:

		CreateBonusPuck(Posx, Posy);

		break;
	case MiniPuck:

		CreateMiniPuck(Posx, Posy);

		break;
	case GhostPuck:

		CreateGhostPuck(Posx, Posy);

		break;
	default:
		break;
	}

#else
	switch (kind)
	{
	case NormalPuck:

		CreateNormalPuck(Posx,Posy);
		
		break;
	case DualPuck:
		
		CreateDualPuck(Posx,Posy);

		break;
	case BonusPuck:

		CreateBonusPuck(Posx,Posy);

		break;
	case MiniPuck:

		CreateMiniPuck(Posx,Posy);

		break;
	case GhostPuck:

		CreateGhostPuck(Posx, Posy);

		break;
	default:
		break;
	}
#endif // _DEBUG

}

// パックの種類の関数
void CreateNormalPuck(float Posx, float Posy)
{
	// 今のパック情報をKindにいれる
	g_Puck[0].Kind = NormalPuck;
	//  パックの位置を入れられたら上下から出す
	g_Puck[0].CurPosx = Posx;
	g_Puck[0].CurPosy = Posy;
	g_Puck[0].radius = PUCKRADIUS / 2.0f;
	g_Puck[0].mass = 2.0f;
	g_Puck[0].Alpha = 1.0f;
	g_Puck[0].color.SetRGB(1.0f, 1.0f, 0.0f);

	// 右に生成されるとき
	if (Posx > SCREEN_WIDTH / 2)
	{
	g_Puck[0].movex = 0.5f;
	g_Puck[0].movey = -2.0f;
	}
	else // 左に生成されるとき
	{
		g_Puck[0].movex = -0.5f;
		g_Puck[0].movey = 2.0f;
	}
	g_Puck[0].exist = true;
	g_Puck[0].Score = 1;
	g_Puck[1].exist = false;

}
void CreateDualPuck(float Posx, float Posy)
{
	for (int i = 0; i < MAX_PUCK; i++)
	{
		g_Puck[i].radius = PUCKRADIUS / 2.0f;
		g_Puck[i].Score = 1;
		g_Puck[i].mass = 2.0f;
		g_Puck[i].Alpha = 1.0f;
		g_Puck[i].color.SetRGB(1.0f, 1.0f, 0.0f);
		g_Puck[i].Kind = DualPuck;
	}

	// 1P側
	g_Puck[0].exist = true;
	g_Puck[0].CurPosx= SCREEN_WIDTH / 2 - g_Puck[0].radius * 2;
	g_Puck[0].CurPosy = 0;
	g_Puck[0].movex = -0.5f;
	g_Puck[0].movey = 2.0f;

	// 2P側
	g_Puck[1].exist = true;
	g_Puck[1].CurPosx = SCREEN_WIDTH / 2 + g_Puck[1].radius * 2;
	g_Puck[1].CurPosy = SCREEN_HEIGHT;
	g_Puck[1].movex = 0.5f;
	g_Puck[1].movey = -2.0f;
}
void CreateBonusPuck(float Posx,float Posy)
{
	//  パックの位置を入れられたら真ん中から出す
	g_Puck[0].CurPosx = Posx;
	g_Puck[0].CurPosy = Posy;
	g_Puck[0].radius = PUCKRADIUS / 2.0f;
	g_Puck[0].mass = 2.0f;
	g_Puck[0].Alpha = 1.0f;
	g_Puck[0].color.SetHSV(1.0f, 1.0f, 1.0f);
	g_Puck[0].Kind = BonusPuck;

	// 右に生成されるとき
	if (Posx > SCREEN_WIDTH / 2)
	{
		g_Puck[0].movex = 0.5f;
		g_Puck[0].movey = -2.0f;
	}
	else // 左に生成されるとき
	{
		g_Puck[0].movex = -0.5f;
		g_Puck[0].movey = 2.0f;
	}
	g_Puck[0].exist = true;
	g_Puck[0].Score = BONUSSCORE;
	g_Puck[1].exist = false;
}
void CreateMiniPuck(float Posx, float Posy)
{
	g_Puck[0].CurPosx = Posx;
	g_Puck[0].CurPosy = Posy;
	g_Puck[0].radius = PUCKRADIUS / 4.0f;
	g_Puck[0].Alpha = 1.0f;
	g_Puck[0].color.SetRGB(1.0f, 1.0f, 0.0f);
	g_Puck[0].Kind = MiniPuck;
	// 右に生成されるとき
	if (Posx > SCREEN_WIDTH / 2)
	{
		g_Puck[0].movex = 0.5f;
		g_Puck[0].movey = -2.0f;
	}
	else // 左に生成されるとき
	{
		g_Puck[0].movex = -0.5f;
		g_Puck[0].movey = 2.0f;
	}
	g_Puck[0].mass = 1.0f;
	g_Puck[0].exist = true;
	g_Puck[0].Score = 1;
	g_Puck[1].exist = false;
}
void CreateGhostPuck(float Posx, float Posy)
{
	//  パックの位置を入れられたら上下から出す
	g_Puck[0].CurPosx = Posx;
	g_Puck[0].CurPosy = Posy;
	g_Puck[0].radius = PUCKRADIUS / 2.0f;
	g_Puck[0].mass = 2.0f;
	g_Puck[0].Alpha = 1.0f;
	g_Puck[0].color.SetRGB(210.0f / 255.0f, 204.0f / 255.0f, 230.0f / 255.0f);
	g_Puck[0].Kind = GhostPuck;
	// 右に生成されるとき
	if (Posx > SCREEN_WIDTH / 2)
	{
		g_Puck[0].movex = 0.5f;
		g_Puck[0].movey = -2.0f;
	}
	else // 左に生成されるとき
	{
		g_Puck[0].movex = -0.5f;
		g_Puck[0].movey = 2.0f;
	}
	g_Puck[0].exist = true;
	g_Puck[0].Score = 1;
	g_Puck[1].exist = false;

}
void CreateMaxPuck()
{
	for (int i = 0; i < MAX_PUCK; i++)
	{
		g_Puck[i].exist = true;
		g_Puck[i].CurPosx = RANDOM(0, SCREEN_WIDTH);
		g_Puck[i].CurPosy = RANDOM(0, SCREEN_HEIGHT);
	}
}








// 衝突時に呼ぶ
// aをパック bをパックとする
void PuckColProcess(PuckInfo *a, PuckInfo *b)
{
	// 無いパックの当たり判定を取らないようにする処置
	if (b->exist == false||a->exist==false)
	{
		return;
	}

	//--- めり込んだ分パックを移動する
	// めり込んだ量を調べる
	float vx = (a->CurPosx - b->CurPosx);
	float vy = (a->CurPosy - b->CurPosy);
	float length = sqrtf(vx*vx + vy * vy);
	float distance = (a->radius + b->radius) - length;
	// aの補正方向を調べる
	if (length > 0)
		length = 1.0f / length;
	distance *= 0.5f;
	distance *= 1.0001f;
	vx *= length;
	vy *= length;
	// aをめり込まない位置まで補正
	a->CurPosx += vx * distance;
	a->CurPosy += vy * distance;
	// bをめり込まない位置まで補正する
	b->CurPosx -= vx * distance;
	b->CurPosy -= vy * distance;

	//--- 衝突後の方向を調べる
	float t;	// 方向
	float arX;	// aがbに与える移動運動X方向
	float arY;
	float amX;	// aがbに与える回転運動X方向
	float amY;
	float brX;	// bがaに与える移動運動X方向
	float brY;
	float bmX;	// bがaに与える回転運動
	float bmY;

	vx *= -1;
	vy *= -1;


	// 移動運動
	t = -(vx * a->movex + vy * a->movey) / (vx * vx + vy * vy);
	arX = a->movex + vx * t;
	arY = a->movey + vy * t;

	// 回転運動
	t = -(-vy * a->movex + vx * a->movey) / (vy * vy + vx * vx);
	amX = a->movex - vy * t;
	amY = a->movey + vx * t;

	// 移動運動
	t = -(vx * b->movex + vy * b->movey) / (vx * vx + vy * vy);
	brX = b->movex + vx * t;
	brY = b->movey + vy * t;

	// 回転運動
	t = -(-vy * b->movex + vx * b->movey) / (vy * vy + vx * vx);
	bmX = b->movex - vy * t;
	bmY = b->movey + vx * t;

	//--- 反発させる
	float e = 1.0f;		// 反発係数
	float avX;			// 反発後のaの速度X方向
	float avY;
	float bvX;			// 反発後のbの速度X方向
	float bvY;

	avX = (a->mass * amX + b->mass * bmX + bmX * e * b->mass - amX * e * b->mass) / (a->mass + b->mass);
	avY = (a->mass * amY + b->mass * bmY + bmY * e * b->mass - amY * e * b->mass) / (a->mass + b->mass);
	bvX = -e * (bmX - amX) + avX;
	bvY = -e * (bmY - amY) + avY;

	// 速度反映
	a->movex = avX + arX;
	a->movey = avY + arY;
	b->movex = bvX + brX;
	b->movey = bvY + brY;

	//----- 当たった点からパーティクルを出す -----
	Float2 hitPos;
	float fHitRad;

	fHitRad = atan2f(b->CurPosy - a->CurPosy, b->CurPosx - a->CurPosx);
	hitPos.x = a->CurPosx + cosf(fHitRad)*a->radius;
	hitPos.y = a->CurPosy + sinf(fHitRad)*a->radius;
	myVector2 vec1(a->movex, a->movey);
	myVector2 vec2(b->movex, b->movey);
	myVector2 vec3 = vec1 - vec2;
	float RelativeSpd = vec3.GetLength();		// 相対速度
	// パーティクルエフェクトの作成
	ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
	if (pEmitter != nullptr)
	{
		Float4 Color(a->color.GetRBG().x, a->color.GetRBG().y, a->color.GetRBG().z, 1.0f);
		pEmitter->CreateParticle(30);
		pEmitter->SetColor(Color);
		pEmitter->SetLife(RelativeSpd*1.0f);
		pEmitter->SetPos(hitPos);
		pEmitter->SetSize(10);
		pEmitter->SetSpd(RelativeSpd / 2.0f);
		pEmitter->SetRadRange(0, 2 * PI);
	}
}
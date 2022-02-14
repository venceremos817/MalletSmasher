
#include "mallet.h"
#include "Source.h"
#include "Texture.h"
#include "myPolygon.h"
#include "Keyboard.h"
#include "Puck.h"
#include "Collision.h"
#include <math.h>
#include "JoyPad.h"
#include "game.h"
#include "CMyPolygon.h"
#include "SoundEffect.h"
#include "myVector.h"

// 定数
#define MALLETMASS (5.0f)
#define MALLET_DEF_POWER (0.9f)
#define SMASH_POWER		(1.5f)
#define PI (3.14159265359)
#define HIT_STOP_FRAME	(5)
#define SMASH_CHARGE_FRAME	(45)	// スマッシュに必要な溜め時間	(ガイルのソニックと同じ溜め時間)
#define SMASH_FRAME			(5)		// スマッシュ持続フレーム
#define SMASH_SPEED		(MALLET_SPEED*2.0f)


// 関数
void InitMallet_1P();
void InitMallet_2P();
void UpdateMallet_1P();
void UpdateMallet_2P();
void DrawMallet_1P();
void DrawMallet_2P();

// パックと当たった時の処理
void CircleColProcess(MalletInfo *a, PuckInfo *b);



// グローバル変数
MalletInfo g_mallet_1P;
MalletInfo g_mallet_2P;
ID3D11ShaderResourceView *g_pTexMallet;
ID3D11ShaderResourceView *g_pTexBright;
IXAudio2SourceVoice *g_pMalletSESpeaker;




HRESULT InitMallet()
{
	HRESULT hr = S_OK;
	InitMallet_1P();
	InitMallet_2P();

	// テクスチャ読み込み
	hr = LoadTextureFromFile("image/Circle.png", &g_pTexMallet);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = LoadTextureFromFile("image/bright.png", &g_pTexBright);
	if (FAILED(hr))
	{
		return hr;
	}


	return hr;

}
void UninitMallet()
{
	// テクスチャの開放
	SAFE_RELEASE(g_pTexMallet);

	if (g_pMalletSESpeaker != NULL)
		g_pMalletSESpeaker->Stop();
}
void UpdateMallet()
{
	UpdateMallet_1P();
	UpdateMallet_2P();
}
void DrawMallet()
{
	DrawMallet_1P();
	DrawMallet_2P();
}
void InitMallet_1P()
{
	g_mallet_1P.CurPosx = 180;
	g_mallet_1P.CurPosy = SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4;
	g_mallet_1P.PrePosx = g_mallet_1P.CurPosx;
	g_mallet_1P.PrePosy = g_mallet_1P.CurPosy;
	g_mallet_1P.movex = 0.0f;
	g_mallet_1P.movey = 0.0f;
	g_mallet_1P.color(0.0f, 0.0f, 0.0f, 1.0f);
	g_mallet_1P.Power = MALLET_DEF_POWER;
	g_mallet_1P.SmashButton = false;
	g_mallet_1P.SmashFrame = 0;
	g_mallet_1P.SmashChargeFrame = 0;
	g_mallet_1P.radius = MALLETRADIUS / 2.0f;
	g_mallet_1P.mass = MALLETMASS;
	g_mallet_1P.State = MalletActive;
	g_mallet_1P.HitStopFrame = 0;
}

void InitMallet_2P()
{
	g_mallet_2P.CurPosx = SCREEN_WIDTH - 180;
	g_mallet_2P.CurPosy = SCREEN_HEIGHT /4;
	g_mallet_2P.PrePosx = g_mallet_2P.CurPosx;
	g_mallet_2P.PrePosy = g_mallet_2P.CurPosy;
	g_mallet_2P.movex = 0.0f;
	g_mallet_2P.movey = 0.0f;
	g_mallet_2P.color(0.0f, 0.0f, 0.0f, 1.0f);
	g_mallet_2P.Power = MALLET_DEF_POWER;
	g_mallet_2P.SmashButton = false;
	g_mallet_2P.SmashFrame = 0;
	g_mallet_2P.SmashChargeFrame = 0;
	g_mallet_2P.radius = MALLETRADIUS * 0.5f;
	g_mallet_2P.mass = MALLETMASS;
	g_mallet_2P.State = MalletActive;
	g_mallet_2P.HitStopFrame = 0;

}
void UpdateMallet_1P()
{
	// ヒットストップ処理
	if (g_mallet_1P.HitStopFrame > 0)
	{
		g_mallet_1P.HitStopFrame--;
		return;
	}

	PuckInfo *pPuck = GetPuckInfo();
	static float SmashDirection = 0;

	switch (g_mallet_1P.State)
	{
	case MalletActive:
		XINPUT_STATE PadState = GetGamePadState(0);

		// 更新の度に0にする
		g_mallet_1P.movex = 0;
		g_mallet_1P.movey = 0;

		// 座標退避
		g_mallet_1P.PrePosx = g_mallet_1P.CurPosx;
		g_mallet_1P.PrePosy = g_mallet_1P.CurPosy;

		g_mallet_1P.movex = (float)PadState.Gamepad.sThumbLX / 32768.0f*(float)MALLET_SPEED;
		g_mallet_1P.movey = -(float)PadState.Gamepad.sThumbLY / 32768.0f*(float)MALLET_SPEED;

		// キー入力
		if (IsKeyPress('A'))
		{
			g_mallet_1P.movex -= MALLET_SPEED;
		}
		if (IsKeyPress('D'))
		{
			g_mallet_1P.movex += MALLET_SPEED;
		}
		if (IsKeyPress('W'))
		{
			g_mallet_1P.movey -= MALLET_SPEED;
		}
		if (IsKeyPress('S'))
		{
			g_mallet_1P.movey += MALLET_SPEED;
		}

		// スマッシュ
		g_mallet_1P.SmashButton = false;
		if ((IsKeyPress(VK_LSHIFT) || PadState.Gamepad.wButtons&XINPUT_GAMEPAD_A))
		{
			g_mallet_1P.SmashButton = true;
		}

		// スマッシュボタン押している間
		if (g_mallet_1P.SmashButton == true)
		{
			// チャージ時間
			g_mallet_1P.SmashChargeFrame++;

			// 減速
			g_mallet_1P.movex *= 0.3f;
			g_mallet_1P.movey *= 0.3f;

			// チャージエフェクト
			// パーティクルエフェクトの作成
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(RandRange(g_mallet_1P.CurPosx - g_mallet_1P.radius, g_mallet_1P.CurPosx + g_mallet_1P.radius), RandRange(g_mallet_1P.CurPosy - g_mallet_1P.radius, g_mallet_1P.CurPosy + g_mallet_1P.radius));
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				float rad = atan2f(g_mallet_1P.CurPosy - Pos.y, g_mallet_1P.CurPosx - Pos.x);
				pEmitter->CreateParticle(1);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(10);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(15);
				pEmitter->SetSpd(1);
				pEmitter->SetRad(rad);
				pEmitter->SetTexture(g_pTexBright);
				pEmitter->IsAddBlend(true);
			}

		}

		

		// スマッシュ発動
		if (g_mallet_1P.SmashButton == false && g_mallet_1P.SmashChargeFrame >= SMASH_CHARGE_FRAME)
		{
			// 最初のフレームのみの処理
			SmashDirection = atan2f(g_mallet_1P.movey, g_mallet_1P.movex);		// 入力がなければ右に飛ぶ
			g_mallet_1P.SmashFrame = SMASH_FRAME;
			g_mallet_1P.SmashChargeFrame = 0;
		}
		else if (g_mallet_1P.SmashButton == false && g_mallet_1P.SmashChargeFrame < SMASH_CHARGE_FRAME)
		{
			// チャージ解除
			g_mallet_1P.SmashChargeFrame = 0;
		}

		

		if (g_mallet_1P.SmashFrame > 0)
		{
			g_mallet_1P.SmashFrame--;
			// スマッシュ中は直進する
			g_mallet_1P.movex = cosf(SmashDirection)*SMASH_SPEED;
			g_mallet_1P.movey = sinf(SmashDirection)*SMASH_SPEED;
			g_mallet_1P.color(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			g_mallet_1P.color(0.0f, 0.0f, 0.0f, 1.0f);
		}

		// 座標更新
		g_mallet_1P.CurPosx += g_mallet_1P.movex;
		g_mallet_1P.CurPosy += g_mallet_1P.movey;

		

		// 判定左
		if (g_mallet_1P.CurPosx - g_mallet_1P.radius <= 0)
		{
			g_mallet_1P.CurPosx = 0 + g_mallet_1P.radius;
		}
		// 判定右
		if (g_mallet_1P.CurPosx + g_mallet_1P.radius >= SCREEN_WIDTH / 2)
		{
			g_mallet_1P.CurPosx = SCREEN_WIDTH / 2 - g_mallet_1P.radius;
		}
		// 判定上
		if (g_mallet_1P.CurPosy - g_mallet_1P.radius < 0)
		{
			g_mallet_1P.CurPosy = 0 + g_mallet_1P.radius;
		}
		// 判定下
		if (g_mallet_1P.CurPosy + g_mallet_1P.radius > SCREEN_HEIGHT)
		{
			g_mallet_1P.CurPosy = SCREEN_HEIGHT - g_mallet_1P.radius;
		}

		for (int i = 0; i < MAX_PUCK; i++, pPuck++)
		{
			if (pPuck->exist == false)
				continue;
			// パックとの当たり判定
			if (CheckCollisionCircleToCircle(g_mallet_1P.CurPosx, g_mallet_1P.CurPosy, pPuck->CurPosx, pPuck->CurPosy, g_mallet_1P.radius, pPuck->radius))
			{
				// スマッシュ時
				if (g_mallet_1P.SmashFrame)
				{
					// ヒットストップをかける
					g_mallet_1P.HitStopFrame = HIT_STOP_FRAME;
					pPuck->HitStopFrame = HIT_STOP_FRAME - 1;
					// スマッシュ解除
					myVector2 vec1(g_mallet_1P.PrePosx - g_mallet_1P.CurPosx, g_mallet_1P.PrePosy - g_mallet_1P.CurPosy);
					myVector2 vec2(pPuck->PrePosx - pPuck->CurPosx, pPuck->PrePosy - pPuck->CurPosy);
					myVector2 vec3 = vec2 - vec1;
					float rad = atan2f(pPuck->PrePosy - g_mallet_1P.PrePosy, pPuck->PrePosx - g_mallet_1P.PrePosx);
					float SeVol = vec3.x*cosf(rad) + vec3.y * sinf(rad);
					g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_SMASHHIT_MALLET_AND_PUCK));
					g_pMalletSESpeaker->SetVolume(SeVol / MALLET_SPEED * 2.0f);
				}
				else	// 非スマッシュ時
				{
					myVector2 vec1(g_mallet_1P.PrePosx - g_mallet_1P.CurPosx, g_mallet_1P.PrePosy - g_mallet_1P.CurPosy);
					myVector2 vec2(pPuck->PrePosx - pPuck->CurPosx, pPuck->PrePosy - pPuck->CurPosy);
					myVector2 vec3 = vec2 - vec1;
					float rad = atan2f(pPuck->PrePosy - g_mallet_1P.PrePosy, pPuck->PrePosx - g_mallet_1P.PrePosx);
					float SeVol = vec3.x*cosf(rad) + vec3.y * sinf(rad);
					g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_HIT_MALLET_AND_PUCK));
					g_pMalletSESpeaker->SetVolume(SeVol / MALLET_SPEED);
				}

				CircleColProcess(&g_mallet_1P, pPuck);

			}
		}

		if (g_mallet_1P.SmashButton == true && g_mallet_1P.SmashChargeFrame == 1)
		{
			g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_CHARGE1));
			g_pMalletSESpeaker->SetVolume(1.0f / 3.0f);
		}
		//チャージ半分
		else if (g_mallet_1P.SmashButton == true && g_mallet_1P.SmashChargeFrame == SMASH_CHARGE_FRAME / 2)
		{
			g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_CHARGE2));
			g_pMalletSESpeaker->SetVolume(2.0f / 3.0f);
		}
		// チャージ溜まりきった瞬間
		else if (g_mallet_1P.SmashButton == true && g_mallet_1P.SmashChargeFrame == SMASH_CHARGE_FRAME)
		{
			// パーティクルエフェクトの作成
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(g_mallet_1P.CurPosx, g_mallet_1P.CurPosy);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(30);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(30);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(30);
				pEmitter->SetSpd(5);
				pEmitter->SetRadRange(0, 2 * PI);
				pEmitter->SetTexture(g_pTexBright);
			}

			g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_CHARGE3));
			g_pMalletSESpeaker->SetVolume(1.0f);
		}

		break;
	case MalletNonActive:
		if (CheckColisionPointToCircle(MALLETDEFAULT_POSX_1P, MALLETDEFAULT_POSY_1P, g_mallet_1P.CurPosx, g_mallet_1P.CurPosy, g_mallet_1P.radius))
			g_mallet_1P.State = MalletActive;
		// 座標退避
		g_mallet_1P.PrePosx = g_mallet_1P.CurPosx;
		g_mallet_1P.PrePosy = g_mallet_1P.CurPosy;
		// 座標更新
		g_mallet_1P.CurPosx += g_mallet_1P.movex;
		g_mallet_1P.CurPosy += g_mallet_1P.movey;

		break;
	default:
		break;
	}



}
void UpdateMallet_2P()
{
	// ヒットストップ処理
	if (g_mallet_2P.HitStopFrame > 0)
	{
		g_mallet_2P.HitStopFrame--;
		return;
	}

	PuckInfo *pPuck = GetPuckInfo();
	static float SmashDirection = PI;
	switch (g_mallet_2P.State)
	{
	case MalletActive:
		XINPUT_STATE PadState = GetGamePadState(1);

		// 更新の度に0にする
		g_mallet_2P.movex = 0;
		g_mallet_2P.movey = 0;
		// 座標退避
		g_mallet_2P.PrePosx = g_mallet_2P.CurPosx;
		g_mallet_2P.PrePosy = g_mallet_2P.CurPosy;

		// コントローラ設定
		g_mallet_2P.movex = (float)PadState.Gamepad.sThumbLX / 32768.0f*(float)MALLET_SPEED;
		g_mallet_2P.movey = -(float)PadState.Gamepad.sThumbLY / 32768.0f*(float)MALLET_SPEED;


		// キー入力
		if (IsKeyPress('K'))	// 左
		{
			g_mallet_2P.movex -= MALLET_SPEED;
		}
		if (IsKeyPress(VK_OEM_PLUS))	// 右
		{
			g_mallet_2P.movex += MALLET_SPEED;
		}
		if (IsKeyPress('O'))	// 上
		{
			g_mallet_2P.movey -= MALLET_SPEED;
		}
		if (IsKeyPress('L'))	// 下
		{
			g_mallet_2P.movey += MALLET_SPEED;
		}

		// スマッシュ
		g_mallet_2P.SmashButton = false;
		if ((IsKeyPress(VK_RSHIFT) || PadState.Gamepad.wButtons&XINPUT_GAMEPAD_A))
		{
			g_mallet_2P.SmashButton = true;
		}

		// スマッシュボタン押している間
		if (g_mallet_2P.SmashButton == true)
		{
			// チャージ時間
			g_mallet_2P.SmashChargeFrame++;

			// 減速
			g_mallet_2P.movex *= 0.3f;
			g_mallet_2P.movey *= 0.3f;

			// チャージエフェクト
			// パーティクルエフェクトの作成
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(RandRange(g_mallet_2P.CurPosx - g_mallet_2P.radius, g_mallet_2P.CurPosx + g_mallet_2P.radius), RandRange(g_mallet_2P.CurPosy - g_mallet_2P.radius, g_mallet_2P.CurPosy + g_mallet_2P.radius));
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				float rad = atan2f(g_mallet_2P.CurPosy - Pos.y, g_mallet_2P.CurPosx - Pos.x);
				pEmitter->CreateParticle(1);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(10);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(15);
				pEmitter->SetSpd(1);
				pEmitter->SetRad(rad);
				pEmitter->SetTexture(g_pTexBright);
				pEmitter->IsAddBlend(true);
			}
		}

		// スマッシュ発動
		if (g_mallet_2P.SmashButton == false && g_mallet_2P.SmashChargeFrame >= SMASH_CHARGE_FRAME)
		{
			// 最初のフレームのみの処理
			SmashDirection = atan2f(g_mallet_2P.movey, g_mallet_2P.movex);
			if (g_mallet_2P.movex == 0.0f&&g_mallet_2P.movey == 0.0f)	// 入力がなければ左に飛ぶ
				SmashDirection = PI;
			g_mallet_2P.SmashFrame = SMASH_FRAME;
			g_mallet_2P.SmashChargeFrame = 0;
		}
		else if (g_mallet_2P.SmashButton == false && g_mallet_2P.SmashChargeFrame < SMASH_CHARGE_FRAME)
		{
			// チャージ解除
			g_mallet_2P.SmashChargeFrame = 0;
		}

		//// チャージ溜まりきった瞬間
		//if (g_mallet_2P.SmashButton == true && g_mallet_2P.SmashChargeFrame == SMASH_CHARGE_FRAME)
		//{
		//	// パーティクルエフェクトの作成
		//	ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
		//	if (pEmitter != nullptr)
		//	{
		//		Float2 Pos(g_mallet_2P.CurPosx, g_mallet_2P.CurPosy);
		//		Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
		//		pEmitter->CreateParticle(1000);
		//		pEmitter->SetColor(Color);
		//		pEmitter->SetLife(20);
		//		pEmitter->SetPos(Pos);
		//		pEmitter->SetSize(5);
		//		pEmitter->SetSpd(10);
		//		pEmitter->SetRadRange(0, 2 * PI);
		//	}
		//}

		if (g_mallet_2P.SmashFrame > 0)
		{
			g_mallet_2P.SmashFrame--;
			// スマッシュ中は直進する
			g_mallet_2P.movex = cosf(SmashDirection)*SMASH_SPEED;
			g_mallet_2P.movey = sinf(SmashDirection)*SMASH_SPEED;
			g_mallet_2P.color(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			g_mallet_2P.color(0.0f, 0.0f, 0.0f, 1.0f);
		}

		// 座標更新
		g_mallet_2P.CurPosx += g_mallet_2P.movex;
		g_mallet_2P.CurPosy += g_mallet_2P.movey;


		// 判定右
		if (g_mallet_2P.CurPosx + g_mallet_2P.radius >= SCREEN_WIDTH)
		{
			g_mallet_2P.CurPosx = SCREEN_WIDTH - g_mallet_2P.radius;
		}
		// 左
		if (g_mallet_2P.CurPosx - g_mallet_2P.radius <= SCREEN_WIDTH / 2)
		{
			g_mallet_2P.CurPosx = SCREEN_WIDTH / 2 + g_mallet_2P.radius;
		}
		// 上
		if (g_mallet_2P.CurPosy - g_mallet_2P.radius < 0)
		{
			g_mallet_2P.CurPosy = 0 + g_mallet_2P.radius;
		}
		// 下
		if (g_mallet_2P.CurPosy + g_mallet_2P.radius > SCREEN_HEIGHT)
		{
			g_mallet_2P.CurPosy = SCREEN_HEIGHT - g_mallet_2P.radius;
		}

		for (int i = 0; i < MAX_PUCK; i++, pPuck++)
		{
			if (pPuck->exist == false)
				continue;

			// パックとの当たり判定
			if (CheckCollisionCircleToCircle(g_mallet_2P.CurPosx, g_mallet_2P.CurPosy, pPuck->CurPosx, pPuck->CurPosy, g_mallet_2P.radius, pPuck->radius))
			{
				// スマッシュ時
				if (g_mallet_2P.SmashFrame)
				{
					// ヒットストップをかける
					g_mallet_2P.HitStopFrame = HIT_STOP_FRAME;
					pPuck->HitStopFrame = HIT_STOP_FRAME - 1;
					// スマッシュ解除
					myVector2 vec1(g_mallet_2P.PrePosx - g_mallet_2P.CurPosx, g_mallet_2P.PrePosy - g_mallet_2P.CurPosy);
					myVector2 vec2(pPuck->PrePosx - pPuck->CurPosx, pPuck->PrePosy - pPuck->CurPosy);
					myVector2 vec3 = vec2 - vec1;
					float rad = atan2f(pPuck->PrePosy - g_mallet_2P.PrePosy, pPuck->PrePosx - g_mallet_2P.PrePosx);
					float SeVol = vec3.x*cosf(rad) + vec3.y * sinf(rad);
					g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_SMASHHIT_MALLET_AND_PUCK));
					g_pMalletSESpeaker->SetVolume(SeVol / MALLET_SPEED * 2.0f);
				}
				else	// 非スマッシュ時
				{
					myVector2 vec1(g_mallet_2P.PrePosx - g_mallet_2P.CurPosx, g_mallet_2P.PrePosy - g_mallet_2P.CurPosy);
					myVector2 vec2(pPuck->PrePosx - pPuck->CurPosx, pPuck->PrePosy - pPuck->CurPosy);
					myVector2 vec3 = vec2 - vec1;
					float rad = atan2f(pPuck->PrePosy - g_mallet_2P.PrePosy, pPuck->PrePosx - g_mallet_2P.PrePosx);
					float SeVol = vec3.x*cosf(rad) + vec3.y * sinf(rad);
					g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_HIT_MALLET_AND_PUCK));
					g_pMalletSESpeaker->SetVolume(SeVol / MALLET_SPEED);
				}

				CircleColProcess(&g_mallet_2P, pPuck);
			}
		}

		if (g_mallet_2P.SmashButton == true && g_mallet_2P.SmashChargeFrame == 1)
		{
			g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_CHARGE1));
			g_pMalletSESpeaker->SetVolume(1.0f / 3.0f);
		}
		//チャージ半分
		else if (g_mallet_2P.SmashButton == true && g_mallet_2P.SmashChargeFrame == SMASH_CHARGE_FRAME / 2)
		{
			g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_CHARGE2));
			g_pMalletSESpeaker->SetVolume(2.0f / 3.0f);
		}
		// チャージ溜まりきった瞬間
		else if (g_mallet_2P.SmashButton == true && g_mallet_2P.SmashChargeFrame == SMASH_CHARGE_FRAME)
		{
			// パーティクルエフェクトの作成
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(g_mallet_2P.CurPosx, g_mallet_2P.CurPosy);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(30);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(30);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(30);
				pEmitter->SetSpd(5);
				pEmitter->SetRadRange(0, 2 * PI);
				pEmitter->SetTexture(g_pTexBright);
			}

			g_pMalletSESpeaker = PlaySound(GetSoundEffect(SE_CHARGE3));
			g_pMalletSESpeaker->SetVolume(1.0f);
		}

		break;
	case MalletNonActive:
		if (CheckColisionPointToCircle(MALLETDEFAULT_POSX_2P, MALLETDEFAULT_POSY_2P, g_mallet_2P.CurPosx, g_mallet_2P.CurPosy, g_mallet_2P.radius))
			g_mallet_2P.State = MalletActive;
		// 座標退避
		g_mallet_2P.PrePosx = g_mallet_2P.CurPosx;
		g_mallet_2P.PrePosy = g_mallet_2P.CurPosy;
		// 座標更新
		g_mallet_2P.CurPosx += g_mallet_2P.movex;
		g_mallet_2P.CurPosy += g_mallet_2P.movey;

		break;
	default:
		break;
	}
	

}
void DrawMallet_1P()
{
	// ポリゴンを表示
	// テクスチャをポリゴンに張り付ける
	SetPolygonTexture(g_pTexMallet);
	CMyPolygon polygon;
	polygon.SetSplitUV(1, 2, 2);
	polygon.SetSize(g_mallet_1P.radius, g_mallet_1P.radius);
	polygon.SetColor((g_mallet_1P.SmashChargeFrame / (float)SMASH_CHARGE_FRAME)*1.0f, 0.0f, 0.0f, 1.0f);
	polygon.SetVtxColor(2, g_mallet_1P.SmashChargeFrame / (float)SMASH_CHARGE_FRAME*2.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		polygon.SetPos(g_mallet_1P.CurPosx - cosf((-3.0f + i * 2.0f) / 4.0f*PI)*g_mallet_1P.radius / sqrtf(2.0f), g_mallet_1P.CurPosy + sinf((-3.0f + i * 2.0f) / 4.0f*PI)*g_mallet_1P.radius / sqrtf(2.0f));
		polygon.SetAngle(-90.0f*i);
		polygon.Draw();
	}

}
void DrawMallet_2P()
{
	// ポリゴンを表示
	// テクスチャをポリゴンに張り付ける
	SetPolygonTexture(g_pTexMallet);
	CMyPolygon polygon;
	polygon.SetSplitUV(1, 2, 2);
	polygon.SetSize(g_mallet_2P.radius, g_mallet_2P.radius);
	polygon.SetColor(0, 0.0f, (g_mallet_2P.SmashChargeFrame / (float)SMASH_CHARGE_FRAME)*1.0f, 1.0f);
	polygon.SetVtxColor(2, 0, 0.0f, g_mallet_2P.SmashChargeFrame / (float)SMASH_CHARGE_FRAME*2.0f, 1.0f);
	for (int i = 0; i < 4; i++)
	{
		polygon.SetPos(g_mallet_2P.CurPosx - cosf((-3.0f + i * 2.0f) / 4.0f*PI)*g_mallet_2P.radius / sqrtf(2.0f), g_mallet_2P.CurPosy + sinf((-3.0f + i * 2.0f) / 4.0f*PI)*g_mallet_2P.radius / sqrtf(2.0f));
		polygon.SetAngle(-90.0f*i);
		polygon.Draw();
	}


}




// 衝突時に呼ぶ
// aをマレット bをパックとする
void CircleColProcess(MalletInfo *a, PuckInfo *b)
{
	// 無いパックの当たり判定を取らないようにする処置
	if (b->exist == false)
	{
		return;
	}

	float aMoveX;	// aの移動量X
	float aMoveY;	// 
	// 移動した分をマレットの移動ベクトルとする
	aMoveX = (a->CurPosx - a->PrePosx)*a->Power;
	aMoveY = (a->CurPosy - a->PrePosy)*a->Power;

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
	distance *= 1.001f;
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
	t = -(vx * aMoveX + vy * aMoveY) / (vx * vx + vy * vy);
	arX = aMoveX + vx * t;
	arY = aMoveY + vy * t;

	// 回転運動
	t = -(-vy * aMoveX + vx * aMoveY) / (vy * vy + vx * vx);
	amX = aMoveX - vy * t;
	amY = aMoveY + vx * t;

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
	//a->movex = avX + arX;
	//a->movey = avY + arY;
	b->movex = bvX + brX;
	b->movey = bvY + brY;

	//----- 当たった点からパーティクルを出す -----
	Float2 hitPos;
	float fHitRad;

	fHitRad = atan2f(b->CurPosy - a->CurPosy, b->CurPosx - a->CurPosx);
	hitPos.x = a->CurPosx + cosf(fHitRad)*a->radius;
	hitPos.y = a->CurPosy + sinf(fHitRad)*a->radius;
	myVector2 vec1(aMoveX, aMoveY);
	myVector2 vec2(b->movex, b->movey);
	myVector2 vec3 = vec1 - vec2;
	float RelativeSpd = vec3.GetLength();	// 相対速度
	// パーティクルエフェクトの作成
	ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
	if (pEmitter != nullptr)
	{
		Float4 Color(a->color.x , a->color.y , a->color.z , 1.0f);
		pEmitter->CreateParticle(RelativeSpd);
		pEmitter->SetColor(Color);
		pEmitter->SetLife(RelativeSpd*1.5f);
		pEmitter->SetPos(hitPos);
		pEmitter->SetSize(7);
		pEmitter->SetSpd(RelativeSpd/3.0f);
		pEmitter->SetRadRange(0, 2 * PI);
		pEmitter->SetTexture(g_pTexMallet);
	}
}

MalletInfo *GetMallet_1P()
{
	return &g_mallet_1P;
}

MalletInfo *GetMallet_2P()
{
	return &g_mallet_2P;
}






ID3D11ShaderResourceView *GetCircleTex()
{
	return g_pTexMallet;
}
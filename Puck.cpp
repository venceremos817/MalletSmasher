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


// �萔 �}�N����`
#define BONUSSCORE (3)
#define PI (3.1415)
#define RANDOM(min,max) (min+(rand()*max-min+1.0)/(1.0+RAND_MAX))


// �֐�
void CreateNormalPuck(float Posx,float Posy);
void CreateDualPuck(float Posx, float Posy);
void CreateBonusPuck(float Posx, float Posy);
void CreateMiniPuck(float Posx, float Posy);
void CreateGhostPuck(float Posx, float Posy);
void PuckColProcess(PuckInfo *a, PuckInfo *b);


// ����΂�
ID3D11ShaderResourceView *g_pPuckTex;
ID3D11ShaderResourceView *g_pPuckAfterTex;
PuckInfo g_Puck[MAX_PUCK];
IXAudio2SourceVoice *g_pPuckSESpeaker;
int g_rad;




HRESULT InitPuck()
{
	// �����_���̏�����
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
	// �e�N�X�`���̊J��
	SAFE_RELEASE(g_pPuckTex);
	SAFE_RELEASE(g_pPuckAfterTex);
}
void UpdatePuck()
{

	for (int i = 0; i < MAX_PUCK; i++)
	{
		if (g_Puck[i].exist == false)
		{
			// for�̐擪�ɖ߂�
			continue;

		}
		if (g_Puck[i].HitStopFrame > 0)
		{
			g_Puck[i].HitStopFrame--;
			continue;
		}

		// ���l�̐ݒ�
		g_rad += 1.0f;

		// ���C
		g_Puck[i].movex *= 0.999f;
		g_Puck[i].movey *= 0.999f;

		// ���W�ޔ�
		g_Puck[i].PrePosx = g_Puck[i].CurPosx;
		g_Puck[i].PrePosy = g_Puck[i].CurPosy;

		// �c���X�V
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

		// ���x����
		if (g_Puck[i].Kind != MiniPuck)
		{
			if (((g_Puck[i].movex)*(g_Puck[i].movex) + (g_Puck[i].movey)*(g_Puck[i].movey)) > (MAX_PACK_SPEED * MAX_PACK_SPEED))
			{
				g_Puck[i].movex = (g_Puck[i].movex * MAX_PACK_SPEED) / (fabsf(g_Puck[i].movex) + fabsf(g_Puck[i].movey));
				g_Puck[i].movey = (g_Puck[i].movey * MAX_PACK_SPEED) / (fabsf(g_Puck[i].movex) + fabsf(g_Puck[i].movey));
			}
		}

		// ���W�X�V
		g_Puck[i].CurPosx += g_Puck[i].movex;
		g_Puck[i].CurPosy += g_Puck[i].movey;


		// �p�b�N���m�̓����蔻��
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

		// �S�[�X�g�p�b�N�̃��l�ݒ�
		if (g_Puck[i].Kind == GhostPuck)
		{
			float rad = 0.0f;
			rad = sinf(g_rad*2.0f / 180 * PI)*0.5f + 0.5f;
			g_Puck[i].Alpha = rad;
		}


		if (g_Puck[i].Kind == BonusPuck)
		{
			// �p�b�N����p�[�e�B�N���o��
			// �O��
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

			// �Q�[�~���O�p�b�N
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

		// �c���`��
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



		// �p�b�N�`��
		SetPolygonTexture(g_pPuckAfterTex);

		SetPolygonVtxColor(0, g_Puck[i].color.GetRed(), g_Puck[i].color.GetGreen(), g_Puck[i].color.GetBlue(), g_Puck[i].Alpha);
		SetPolygonVtxColor(1, g_Puck[i].color.GetRed(), g_Puck[i].color.GetGreen(), g_Puck[i].color.GetBlue(), g_Puck[i].Alpha);
		SetPolygonVtxColor(2, g_Puck[i].color.GetRed(), g_Puck[i].color.GetGreen(), g_Puck[i].color.GetBlue(), g_Puck[i].Alpha);
		SetPolygonVtxColor(3, g_Puck[i].color.GetRed(), g_Puck[i].color.GetGreen(), g_Puck[i].color.GetBlue(), g_Puck[i].Alpha);

		SetPolygonPos(g_Puck[i].CurPosx, g_Puck[i].CurPosy);
		SetPolygonSize(g_Puck[i].radius*2.0f, g_Puck[i].radius*2.0f);

		SetPolygonAngle(0.0f);

		// �`��
		DrawPolygon();

	}
}

// �p�b�N���擾
PuckInfo *GetPuckInfo()
{
	return g_Puck;
}
// �p�b�N����
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

// �p�b�N�̎�ނ̊֐�
void CreateNormalPuck(float Posx, float Posy)
{
	// ���̃p�b�N����Kind�ɂ����
	g_Puck[0].Kind = NormalPuck;
	//  �p�b�N�̈ʒu������ꂽ��㉺����o��
	g_Puck[0].CurPosx = Posx;
	g_Puck[0].CurPosy = Posy;
	g_Puck[0].radius = PUCKRADIUS / 2.0f;
	g_Puck[0].mass = 2.0f;
	g_Puck[0].Alpha = 1.0f;
	g_Puck[0].color.SetRGB(1.0f, 1.0f, 0.0f);

	// �E�ɐ��������Ƃ�
	if (Posx > SCREEN_WIDTH / 2)
	{
	g_Puck[0].movex = 0.5f;
	g_Puck[0].movey = -2.0f;
	}
	else // ���ɐ��������Ƃ�
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

	// 1P��
	g_Puck[0].exist = true;
	g_Puck[0].CurPosx= SCREEN_WIDTH / 2 - g_Puck[0].radius * 2;
	g_Puck[0].CurPosy = 0;
	g_Puck[0].movex = -0.5f;
	g_Puck[0].movey = 2.0f;

	// 2P��
	g_Puck[1].exist = true;
	g_Puck[1].CurPosx = SCREEN_WIDTH / 2 + g_Puck[1].radius * 2;
	g_Puck[1].CurPosy = SCREEN_HEIGHT;
	g_Puck[1].movex = 0.5f;
	g_Puck[1].movey = -2.0f;
}
void CreateBonusPuck(float Posx,float Posy)
{
	//  �p�b�N�̈ʒu������ꂽ��^�񒆂���o��
	g_Puck[0].CurPosx = Posx;
	g_Puck[0].CurPosy = Posy;
	g_Puck[0].radius = PUCKRADIUS / 2.0f;
	g_Puck[0].mass = 2.0f;
	g_Puck[0].Alpha = 1.0f;
	g_Puck[0].color.SetHSV(1.0f, 1.0f, 1.0f);
	g_Puck[0].Kind = BonusPuck;

	// �E�ɐ��������Ƃ�
	if (Posx > SCREEN_WIDTH / 2)
	{
		g_Puck[0].movex = 0.5f;
		g_Puck[0].movey = -2.0f;
	}
	else // ���ɐ��������Ƃ�
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
	// �E�ɐ��������Ƃ�
	if (Posx > SCREEN_WIDTH / 2)
	{
		g_Puck[0].movex = 0.5f;
		g_Puck[0].movey = -2.0f;
	}
	else // ���ɐ��������Ƃ�
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
	//  �p�b�N�̈ʒu������ꂽ��㉺����o��
	g_Puck[0].CurPosx = Posx;
	g_Puck[0].CurPosy = Posy;
	g_Puck[0].radius = PUCKRADIUS / 2.0f;
	g_Puck[0].mass = 2.0f;
	g_Puck[0].Alpha = 1.0f;
	g_Puck[0].color.SetRGB(210.0f / 255.0f, 204.0f / 255.0f, 230.0f / 255.0f);
	g_Puck[0].Kind = GhostPuck;
	// �E�ɐ��������Ƃ�
	if (Posx > SCREEN_WIDTH / 2)
	{
		g_Puck[0].movex = 0.5f;
		g_Puck[0].movey = -2.0f;
	}
	else // ���ɐ��������Ƃ�
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








// �Փˎ��ɌĂ�
// a���p�b�N b���p�b�N�Ƃ���
void PuckColProcess(PuckInfo *a, PuckInfo *b)
{
	// �����p�b�N�̓����蔻������Ȃ��悤�ɂ��鏈�u
	if (b->exist == false||a->exist==false)
	{
		return;
	}

	//--- �߂荞�񂾕��p�b�N���ړ�����
	// �߂荞�񂾗ʂ𒲂ׂ�
	float vx = (a->CurPosx - b->CurPosx);
	float vy = (a->CurPosy - b->CurPosy);
	float length = sqrtf(vx*vx + vy * vy);
	float distance = (a->radius + b->radius) - length;
	// a�̕␳�����𒲂ׂ�
	if (length > 0)
		length = 1.0f / length;
	distance *= 0.5f;
	distance *= 1.0001f;
	vx *= length;
	vy *= length;
	// a���߂荞�܂Ȃ��ʒu�܂ŕ␳
	a->CurPosx += vx * distance;
	a->CurPosy += vy * distance;
	// b���߂荞�܂Ȃ��ʒu�܂ŕ␳����
	b->CurPosx -= vx * distance;
	b->CurPosy -= vy * distance;

	//--- �Փˌ�̕����𒲂ׂ�
	float t;	// ����
	float arX;	// a��b�ɗ^����ړ��^��X����
	float arY;
	float amX;	// a��b�ɗ^�����]�^��X����
	float amY;
	float brX;	// b��a�ɗ^����ړ��^��X����
	float brY;
	float bmX;	// b��a�ɗ^�����]�^��
	float bmY;

	vx *= -1;
	vy *= -1;


	// �ړ��^��
	t = -(vx * a->movex + vy * a->movey) / (vx * vx + vy * vy);
	arX = a->movex + vx * t;
	arY = a->movey + vy * t;

	// ��]�^��
	t = -(-vy * a->movex + vx * a->movey) / (vy * vy + vx * vx);
	amX = a->movex - vy * t;
	amY = a->movey + vx * t;

	// �ړ��^��
	t = -(vx * b->movex + vy * b->movey) / (vx * vx + vy * vy);
	brX = b->movex + vx * t;
	brY = b->movey + vy * t;

	// ��]�^��
	t = -(-vy * b->movex + vx * b->movey) / (vy * vy + vx * vx);
	bmX = b->movex - vy * t;
	bmY = b->movey + vx * t;

	//--- ����������
	float e = 1.0f;		// �����W��
	float avX;			// �������a�̑��xX����
	float avY;
	float bvX;			// �������b�̑��xX����
	float bvY;

	avX = (a->mass * amX + b->mass * bmX + bmX * e * b->mass - amX * e * b->mass) / (a->mass + b->mass);
	avY = (a->mass * amY + b->mass * bmY + bmY * e * b->mass - amY * e * b->mass) / (a->mass + b->mass);
	bvX = -e * (bmX - amX) + avX;
	bvY = -e * (bmY - amY) + avY;

	// ���x���f
	a->movex = avX + arX;
	a->movey = avY + arY;
	b->movex = bvX + brX;
	b->movey = bvY + brY;

	//----- ���������_����p�[�e�B�N�����o�� -----
	Float2 hitPos;
	float fHitRad;

	fHitRad = atan2f(b->CurPosy - a->CurPosy, b->CurPosx - a->CurPosx);
	hitPos.x = a->CurPosx + cosf(fHitRad)*a->radius;
	hitPos.y = a->CurPosy + sinf(fHitRad)*a->radius;
	myVector2 vec1(a->movex, a->movey);
	myVector2 vec2(b->movex, b->movey);
	myVector2 vec3 = vec1 - vec2;
	float RelativeSpd = vec3.GetLength();		// ���Α��x
	// �p�[�e�B�N���G�t�F�N�g�̍쐬
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
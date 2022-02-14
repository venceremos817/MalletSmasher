#include "Stage.h"
#include "myPolygon.h"
#include "Source.h"
#include "Texture.h"
#include <math.h>
#include "title.h"
#include "Puck.h"
#include "score.h"
#include "Effect.h"
#include "mallet.h"
#include "Conveyor.h"
#include "game.h"
#include "goal.h"
#include "mallet.h"
#include "SoundEffect.h"

#define MAX_CONVEYOR (2)
#define PI (3.141592653592f)
#define CREATEPARTIClLE (30)

// �\����
typedef struct
{
	float Top;
	float Bottom;
	float Left;
	float Right;

}StageInfo;


// �֐�
void UpdateTutorial();
void UpdateNormalStage();
void UpdateTopUnderPairStage();
void UpdateConveyorStage();
void UpdateNormalOnlyStage();

// �O���[�o���ϐ�
ID3D11ShaderResourceView *g_pStageTex;
ID3D11ShaderResourceView *g_pStarTex;
StageInfo g_Stage;
StageKind g_CurStage;
Conveyor *pConveyor = NULL;
//IXAudio2SourceVoice *g_pWallSESpeaker;
IXAudio2SourceVoice *g_pStageBGMSpeaker;
IXAudio2SourceVoice *g_pStageSESpeaker;



HRESULT InitStage(StageKind stagekind)
{
	HRESULT hr = S_OK;

	g_CurStage = stagekind;
	switch (stagekind)
	{
	case NormalStage:
	case TutorialStage:
	case TopUnderPairStage:
	case ConveyorStage:
		g_Stage.Top = 0;
		g_Stage.Bottom = SCREEN_HEIGHT;
		g_Stage.Left = 0;
		g_Stage.Right = SCREEN_WIDTH;

		break;
	case NormalOnlyStage:
	default:
		break;
	}

	switch (stagekind)
	{
	case NormalStage:
		// �e�N�X�`���ǂݍ���
		hr = LoadTextureFromFile("image/bg.png", &g_pStageTex);
		if (FAILED(hr))
			return hr;
		break;
	case TopUnderPairStage:
		// �e�N�X�`���ǂݍ���
		hr = LoadTextureFromFile("image/bg.png", &g_pStageTex);
		if (FAILED(hr))
			return hr;
		break;
	case ConveyorStage:
		pConveyor = new Conveyor[2];
		pConveyor[0].Init();
		pConveyor[1].Init();
		// �R���x�A�̈ʒu
		pConveyor[0].SetPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT * (1.0f / 4.0f));
		pConveyor[1].SetPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT * (3.0f / 4.0f));
		

		hr = LoadTextureFromFile("image/bg.png", &g_pStageTex);
		if (FAILED(hr))
			return hr;
		break;

	case NormalOnlyStage:
		// �e�N�X�`���ǂݍ���
		hr = LoadTextureFromFile("image/bg.png", &g_pStageTex);
		if (FAILED(hr))
			return hr;
		break;
	case TutorialStage:
		// �e�N�X�`���ǂݍ���
		hr = LoadTextureFromFile("menu/TitleBg.png", &g_pStageTex);
		if (FAILED(hr))
			return hr;
		break;
	default:
		break;
	}



	switch (stagekind)
	{
	case NormalStage:	
	case TopUnderPairStage:
	case NormalOnlyStage:
	case ConveyorStage:
	case MaxStage:
		g_pStageBGMSpeaker = PlaySound(GetBGM(BGM_GAME_STAGE));

		break;
	case TutorialStage:
		g_pStageBGMSpeaker = PlaySound(GetBGM(BGM_TUTORIAL_STAGE));

		break;
	default:
		break;
	}

#ifdef _DEBUG
	g_pStageBGMSpeaker->SetVolume(0.0f);
#endif // _DEBUG

	return hr;
}
void UninitStage()
{
	if (pConveyor != NULL)
	{
		pConveyor[0].Uninit();
		pConveyor[1].Uninit();

		delete[] pConveyor;
		pConveyor = NULL;
	}

	g_pStageBGMSpeaker->Stop();

	SAFE_RELEASE(g_pStageTex);
}
void UpdateStage()
{
	switch (g_CurStage)
	{
	case NormalStage:
		UpdateNormalStage();
		break;
	case TopUnderPairStage:
		UpdateTopUnderPairStage();
		break;
	case ConveyorStage:
		UpdateConveyorStage();
		for (int i = 0; i < MAX_CONVEYOR; i++)
		{
			pConveyor[i].Update();
		}
		break;
	case NormalOnlyStage:
		UpdateNormalOnlyStage();
		break;
	case TutorialStage:
		UpdateTutorial();
		break;
	default:
		break;
	}
	
	
}
void DrawStage()
{
	SetPolygonPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetPolygonVtxColor(0, 1, 1, 1, 1);
	SetPolygonVtxColor(1, 1, 1, 1, 1);
	SetPolygonVtxColor(2, 1, 1, 1, 1);
	SetPolygonVtxColor(3, 1, 1, 1, 1);
	SetPolygonTexture(g_pStageTex);

	SetPolygonSplitUV(0, 1, 1);

	DrawPolygon();

	if (pConveyor != NULL)
	{
		for (int i = 0; i < MAX_CONVEYOR; i++)
		{
			pConveyor[i].Draw();
		}
	}
}

void UpdateTutorial()
{
	PuckInfo *pRootPuck = GetPuckInfo();
	PuckInfo *pPuck = pRootPuck;


	// �S�[���̔��˂���Ƃ���̏�̕�
	float GoalTop = (SCREEN_HEIGHT / 2.0f) - (PUCKRADIUS + MALLETRADIUS / 2.0f);
	// �S�[���̔��˂���Ƃ���̉��̕�
	float GoalBottom = (SCREEN_HEIGHT / 2.0f) + (PUCKRADIUS + MALLETRADIUS / 2.0f);

	for (int i = 0; i < MAX_PUCK; i++)
	{

		if (pPuck->exist == false)
		{
			continue;
		}

		// �ǂƂ̔���
		// ���̕�
		if (pPuck->CurPosx - pPuck->radius <= g_Stage.Left)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx - pPuck->radius, pPuck->CurPosy);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(10);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(8);
				pEmitter->SetSpd(fabsf(pPuck->movey) * 0.3f);
				pEmitter->SetRadRange(3.0f*PI / 2.0f, 5.0f*PI / 2.0f);
			}
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx - pPuck->radius, pPuck->CurPosy);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(10);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(5);
				pEmitter->SetSpd(fabsf(pPuck->movey) * 0.15f);
				pEmitter->SetRadRange(3.0f*PI / 2.0f, 5.0f*PI / 2.0f);
			}
			pPuck->CurPosx = g_Stage.Left + (pPuck->radius + 1.0f);
			pPuck->movex *= -1.0f;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(-pPuck->movex / MAX_PACK_SPEED);
		}
		// �E�̕�
		if (pPuck->CurPosx + pPuck->radius >= g_Stage.Right)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx + pPuck->radius, pPuck->CurPosy);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(20);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(8);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.5f)*0.5f);
				pEmitter->SetRadRange(PI / 2.0f, 3.0f*PI / 2.0f);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx + pPuck->radius, pPuck->CurPosy);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(20);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(5);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.5);
				pEmitter->SetRadRange(PI / 2.0f, 3.0f*PI / 2.0f);
			}
			pPuck->CurPosx = g_Stage.Right - (pPuck->radius + 1.0f);
			pPuck->movex *= -1.0f;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(pPuck->movex / MAX_PACK_SPEED);

		}
		// ��̕�
		if (pPuck->CurPosy - pPuck->radius < g_Stage.Top)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(5);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(40);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(6 + 2);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.35f)*0.9);
				pEmitter->SetRadRange(0.0f, PI);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 0.5f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(25);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(8 + 2);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.7);
				pEmitter->SetRadRange(0.0f, PI);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(15);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(10 + 2);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.15f)*0.6);
				pEmitter->SetRadRange(0.0f, PI);
			}

			pPuck->CurPosy = g_Stage.Top + (pPuck->radius + 1.0f);
			pPuck->movey *= -1;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(-pPuck->movey / MAX_PACK_SPEED);

		}
		// ���̕�
		if (pPuck->CurPosy + pPuck->radius > g_Stage.Bottom)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(5);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(40);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(6 + 2);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.35f)*0.9);
				pEmitter->SetRadRange(PI, PI * 2.0f);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 0.5f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(25);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(8 + 2);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.7);
				pEmitter->SetRadRange(PI, PI * 2.0f);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(15);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(10 + 2);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.15f)*0.6f);
				pEmitter->SetRadRange(PI, PI * 2.0f);
			}

			pPuck->CurPosy = g_Stage.Bottom - (pPuck->radius + 1.0f);
			pPuck->movey *= -1;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(pPuck->movey / MAX_PACK_SPEED);

		}
		
		pPuck++;
	}
}

void UpdateNormalStage()
{
	PuckInfo *pRootPuck = GetPuckInfo();
	PuckInfo *pPuck = pRootPuck;

	// �S�[���̔��˂���Ƃ���̏�̕�
	float GoalTop = (SCREEN_HEIGHT / 2.0f) - GOALHEIGHT / 2.0f;//(PUCKRADIUS + MALLETRADIUS / 2.0f);
	// �S�[���̔��˂���Ƃ���̉��̕�
	float GoalBottom = (SCREEN_HEIGHT / 2.0f) + GOALHEIGHT / 2.0f;//(PUCKRADIUS + MALLETRADIUS / 2.0f);

	for (int i = 0; i < MAX_PUCK; i++,pPuck++)
	{
		
		if (pPuck->exist == false)
		{
			continue;
		}

		// �ǂƂ̔���
		// ���̕�
		if (pPuck->CurPosx - pPuck->radius <= g_Stage.Left)
		{
			//	����������
			if (GoalTop >= pPuck->CurPosy - pPuck->radius || GoalBottom <= pPuck->CurPosy + pPuck->radius)
			{
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx - pPuck->radius, pPuck->CurPosy );
					Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(10);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(10);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(8);
					pEmitter->SetSpd(fabsf(pPuck->movey) * 0.3f);
					pEmitter->SetRadRange(3.0f*PI / 2.0f, 5.0f*PI / 2.0f);
				}
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx - pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(15);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(10);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(5);
					pEmitter->SetSpd(fabsf(pPuck->movey) * 0.15f);
					pEmitter->SetRadRange(3.0f*PI / 2.0f, 5.0f*PI / 2.0f);
				}


				pPuck->CurPosx = g_Stage.Left + (pPuck->radius + 1.0f);
				pPuck->movex *= -1.0f;

				g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
				g_pStageSESpeaker->SetVolume(-pPuck->movex / MAX_PACK_SPEED);

			}
			// �S�[���ɓ������Ƃ�
			else if (pPuck->CurPosx + pPuck->radius <= g_Stage.Left)
			{
				// �_�����Z
				AddScore_2P(pPuck->Score);
				// �G�t�F�N�g�̔���
				StartEffect(0);


				// �p�b�N������
				pPuck->exist = false;

				int P = 0;

				for (int u = 0; u < MAX_PUCK; u++)
				{
					// �p�b�N�̐����m�F
					if (pRootPuck[u].exist == true)
					{
						P++;
					}
				}
				if (P > 0)
				{
					continue;
				}

					// �}���b�g�̈ړ�
					MalletInfo *pMallet_1P;
					pMallet_1P = GetMallet_1P();
					pMallet_1P->State = MalletNonActive;
					float MalletRad = atan2f(MALLETDEFAULT_POSY_1P - pMallet_1P->CurPosy, MALLETDEFAULT_POSX_1P - pMallet_1P->CurPosx);
					pMallet_1P->movex = cosf(MalletRad)*15.0f;
					pMallet_1P->movey = sinf(MalletRad)*15.0f;

					// �}���b�g�̈ړ�
					MalletInfo *pMallet_2P;
					pMallet_2P = GetMallet_2P();
					pMallet_2P->State = MalletNonActive;
					MalletRad = atan2f(MALLETDEFAULT_POSY_2P - pMallet_2P->CurPosy, MALLETDEFAULT_POSX_2P - pMallet_2P->CurPosx);
					pMallet_2P->movex = cosf(MalletRad)*15.0f;
					pMallet_2P->movey = sinf(MalletRad)*15.0f;


					// �p�b�N�̐���
					CreatePuck((PuckKind)(rand() % MaxPuckKind), SCREEN_WIDTH / 2 - pPuck->radius * 2, 0);


			}
		}
		// �E�̕�
		if (pPuck->CurPosx + pPuck->radius >= g_Stage.Right)
		{
			// �S�[������
			if (GoalTop >= pPuck->CurPosy - pPuck->radius || GoalBottom <= pPuck->CurPosy + pPuck->radius)
			{
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx + pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(10);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(20);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(8);
					pEmitter->SetSpd((fabsf(pPuck->movey) * 0.5f)*0.5f);
					pEmitter->SetRadRange(PI / 2.0f, 3.0f*PI / 2.0f);
				}

				pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx + pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(15);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(20);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(5);
					pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.5);
					pEmitter->SetRadRange(PI / 2.0f, 3.0f*PI / 2.0f);
				}
				pPuck->CurPosx = g_Stage.Right - (pPuck->radius + 1.0f);
				pPuck->movex *= -1;

				g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
				g_pStageSESpeaker->SetVolume(-pPuck->movex / MAX_PACK_SPEED);

			}
			else if (pPuck->CurPosx - pPuck->radius >= g_Stage.Right)
			{
				// ���_
				AddScore_1P(pPuck->Score);
				// �G�t�F�N�g�̔���
				StartEffect(1);


				// �p�b�N������
				pPuck->exist = false;

				int W = 0;


				for (int u = 0; u < MAX_PUCK; u++)
				{
					// �p�b�N�̐����m�F
					if (pRootPuck[u].exist == true)
					{
						W++;
					}
				}
				if (W > 0)
				{

					continue;

				}
					//�}���b�g�̈ʒu���f�t�H���g�ɖ߂�����
					MalletInfo *pMallet_1P;
					pMallet_1P = GetMallet_1P();
					pMallet_1P->State = MalletNonActive;
					float MalletRad = atan2f(MALLETDEFAULT_POSY_1P - pMallet_1P->CurPosy, MALLETDEFAULT_POSX_1P - pMallet_1P->CurPosx);
					pMallet_1P->movex = cosf(MalletRad)*15.0f;
					pMallet_1P->movey = sinf(MalletRad)*15.0f;

					// �}���b�g�̈ʒu�����ɖ߂�
					MalletInfo *pMallet_2P;
					pMallet_2P = GetMallet_2P();
					pMallet_2P->State = MalletNonActive;
					MalletRad = atan2f(MALLETDEFAULT_POSY_2P - pMallet_2P->CurPosy, MALLETDEFAULT_POSX_2P - pMallet_2P->CurPosx);
					pMallet_2P->movex = cosf(MalletRad)*15.0f;
					pMallet_2P->movey = sinf(MalletRad)*15.0f;

					// �p�b�N�̐���
					CreatePuck((PuckKind)(rand() % MaxPuckKind), SCREEN_WIDTH / 2 + pPuck->radius * 2, SCREEN_HEIGHT);

			}
		}
		// ��̕�
		if (pPuck->CurPosy - pPuck->radius < g_Stage.Top)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(5);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(60);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(10);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.35f)*0.9);
				pEmitter->SetRadRange(0, PI);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 0.5f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(45);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(12);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.7);
				pEmitter->SetRadRange(0, PI);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(35);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(14);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.15f)*0.6);
				pEmitter->SetRadRange(0, PI);
			}
			pPuck->CurPosy = g_Stage.Top + (pPuck->radius + 1.0f);
			pPuck->movey *= -1;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(-pPuck->movey / MAX_PACK_SPEED);

		}
		// ���̕�
		if (pPuck->CurPosy + pPuck->radius > g_Stage.Bottom)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(5);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(60);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(10);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.35f)*0.9);
				pEmitter->SetRadRange(PI, PI*2);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 0.5f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(45);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(12);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.7);
				pEmitter->SetRadRange(PI, PI * 2);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(35);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(12);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.15f)*0.6);
				pEmitter->SetRadRange(PI, PI*2);
			}

			pPuck->CurPosy = g_Stage.Bottom - (pPuck->radius + 1.0f);
			pPuck->movey *= -1;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(pPuck->movey / MAX_PACK_SPEED);

		}

		
	}
}

void UpdateTopUnderPairStage()
{
	PuckInfo *pRootPuck = GetPuckInfo();
	PuckInfo *pPuck = pRootPuck;


	// �S�[���̔��˂���Ƃ���̏�̕�
	float GoalTop = (SCREEN_HEIGHT / 2.0f) - GOALHEIGHT / 2.0f;//(PUCKRADIUS + MALLETRADIUS / 2.0f);
	// �S�[���̔��˂���Ƃ���̉��̕�
	float GoalBottom = (SCREEN_HEIGHT / 2.0f) + GOALHEIGHT / 2.0f;// (PUCKRADIUS + MALLETRADIUS / 2.0f);

	for (int i = 0; i < MAX_PUCK; i++, pPuck++)
	{

		if (pPuck->exist == false)
		{
			continue;
		}

		// �ǂƂ̔���
		// ���̕�
		if (pPuck->CurPosx - pPuck->radius <= g_Stage.Left)
		{

			if (GoalTop >= pPuck->CurPosy - pPuck->radius || GoalBottom <= pPuck->CurPosy + pPuck->radius)
			{
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx - pPuck->radius, pPuck->CurPosy);
					Float4 Color(pPuck->color.GetRed(), pPuck->color.GetGreen(), pPuck->color.GetBlue(), 1.0f);
					pEmitter->CreateParticle(CREATEPARTIClLE);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(10);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(20);
					pEmitter->SetSpd(10);
					pEmitter->SetRadRange(-PI / 4.0f, PI / 4.0f);
				}

				pPuck->CurPosx = g_Stage.Left + (pPuck->radius + 1.0f);
				pPuck->movex *= -1.0f;

				g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
				g_pStageSESpeaker->SetVolume(-pPuck->movex / MAX_PACK_SPEED);

			}
			else if (pPuck->CurPosx + pPuck->radius <= g_Stage.Left)
			{
				// �_�����Z
				AddScore_2P(pPuck->Score);
				// �G�t�F�N�g�̔���
				StartEffect(0);


				// �p�b�N������
				pPuck->exist = false;

				int P = 0;

				for (int u = 0; u < MAX_PUCK; u++)
				{
					// �p�b�N�̐����m�F
					if (pRootPuck[u].exist == true)
					{
						P++;
					}
				}
				if (P > 0)
				{
					continue;
				}

				// �}���b�g�̈ړ�
				MalletInfo *pMallet_1P;
				pMallet_1P = GetMallet_1P();
				pMallet_1P->State = MalletNonActive;
				float MalletRad = atan2f(MALLETDEFAULT_POSY_1P - pMallet_1P->CurPosy, MALLETDEFAULT_POSX_1P - pMallet_1P->CurPosx);
				pMallet_1P->movex = cosf(MalletRad)*15.0f;
				pMallet_1P->movey = sinf(MalletRad)*15.0f;

				// �}���b�g�̈ړ�
				MalletInfo *pMallet_2P;
				pMallet_2P = GetMallet_2P();
				pMallet_2P->State = MalletNonActive;
				MalletRad = atan2f(MALLETDEFAULT_POSY_2P - pMallet_2P->CurPosy, MALLETDEFAULT_POSX_2P - pMallet_2P->CurPosx);
				pMallet_2P->movex = cosf(MalletRad)*15.0f;
				pMallet_2P->movey = sinf(MalletRad)*15.0f;


				// �p�b�N�̐���
				CreatePuck((PuckKind)(rand() % MaxPuckKind), SCREEN_WIDTH / 2 - pPuck->radius * 2, 0);


			}
		}
		// �E�̕�
		if (pPuck->CurPosx + pPuck->radius >= g_Stage.Right)
		{
			// �S�[������
			if (GoalTop >= pPuck->CurPosy - pPuck->radius || GoalBottom <= pPuck->CurPosy + pPuck->radius)
			{
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx + pPuck->radius, pPuck->CurPosy);
					Float4 Color(pPuck->color.GetRed(), pPuck->color.GetGreen(), pPuck->color.GetBlue(), 1.0f);

					pEmitter->CreateParticle(CREATEPARTIClLE);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(10);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(20);
					pEmitter->SetSpd(10);
					pEmitter->SetRadRange(PI *(3.0f / 4.0f), PI *(5.0f / 4.0f));
				}
				pPuck->CurPosx = g_Stage.Right - (pPuck->radius + 1.0f);
				pPuck->movex *= -1;

				g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
				g_pStageSESpeaker->SetVolume(pPuck->movex / MAX_PACK_SPEED);

			}
			else if (pPuck->CurPosx - pPuck->radius >= g_Stage.Right)
			{
				// ���_
				AddScore_1P(pPuck->Score);
				// �G�t�F�N�g�̔���
				StartEffect(1);


				// �p�b�N������
				pPuck->exist = false;

				int W = 0;


				for (int u = 0; u < MAX_PUCK; u++)
				{
					// �p�b�N�̐����m�F
					if (pRootPuck[u].exist == true)
					{
						W++;
					}
				}
				if (W > 0)
				{

					continue;

				}
				//�}���b�g�̈ʒu���f�t�H���g�ɖ߂�����
				MalletInfo *pMallet_1P;
				pMallet_1P = GetMallet_1P();
				pMallet_1P->State = MalletNonActive;
				float MalletRad = atan2f(MALLETDEFAULT_POSY_1P - pMallet_1P->CurPosy, MALLETDEFAULT_POSX_1P - pMallet_1P->CurPosx);
				pMallet_1P->movex = cosf(MalletRad)*15.0f;
				pMallet_1P->movey = sinf(MalletRad)*15.0f;

				// �}���b�g�̈ʒu�����ɖ߂�
				MalletInfo *pMallet_2P;
				pMallet_2P = GetMallet_2P();
				pMallet_2P->State = MalletNonActive;
				MalletRad = atan2f(MALLETDEFAULT_POSY_2P - pMallet_2P->CurPosy, MALLETDEFAULT_POSX_2P - pMallet_2P->CurPosx);
				pMallet_2P->movex = cosf(MalletRad)*15.0f;
				pMallet_2P->movey = sinf(MalletRad)*15.0f;

				// �p�b�N�̐���
				CreatePuck((PuckKind)(rand() % MaxPuckKind), SCREEN_WIDTH / 2 + pPuck->radius * 2, SCREEN_HEIGHT);

			}
		}
		// ��̕�
		if (pPuck->CurPosy < g_Stage.Top)
		{
			pPuck->CurPosy = g_Stage.Bottom;
			pPuck->movex *= 0.8f;
			pPuck->movey *= 0.8f;
			
		}
		// ���̕�
		if (pPuck->CurPosy  > g_Stage.Bottom)
		{
			pPuck->CurPosy = g_Stage.Top;
			pPuck->movex *= 0.8f;
			pPuck->movey *= 0.8f;
		}


	}
}
void UpdateConveyorStage()
{
	PuckInfo *pRootPuck = GetPuckInfo();
	PuckInfo *pPuck = pRootPuck;


	// �S�[���̔��˂���Ƃ���̏�̕�
	float GoalTop = (SCREEN_HEIGHT / 2.0f) - GOALHEIGHT / 2.0f;
	//float GoalTop = (SCREEN_HEIGHT / 2.0f) - (PUCKRADIUS + MALLETRADIUS / 2.0f);
	// �S�[���̔��˂���Ƃ���̉��̕�
	float GoalBottom = (SCREEN_HEIGHT / 2.0f) + GOALHEIGHT / 2.0f;
	//float GoalBottom = (SCREEN_HEIGHT / 2.0f) + (PUCKRADIUS + MALLETRADIUS / 2.0f);

	for (int i = 0; i < MAX_PUCK; i++, pPuck++)
	{

		if (pPuck->exist == false)
		{
			continue;
		}

		// �ǂƂ̔���
		// ���̕�
		if (pPuck->CurPosx - pPuck->radius <= g_Stage.Left)
		{

			if (GoalTop >= pPuck->CurPosy - pPuck->radius || GoalBottom <= pPuck->CurPosy + pPuck->radius)
			{
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx - pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(10);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(10);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(8);
					pEmitter->SetSpd(fabsf(pPuck->movey) * 0.3f);
					pEmitter->SetRadRange(3.0f*PI / 2.0f, 5.0f*PI / 2.0f);
				}
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx - pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(15);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(10);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(5);
					pEmitter->SetSpd(fabsf(pPuck->movey) * 0.15f);
					pEmitter->SetRadRange(3.0f*PI / 2.0f, 5.0f*PI / 2.0f);
				}

				pPuck->CurPosx = g_Stage.Left + (pPuck->radius + 1.0f);
				pPuck->movex *= -1.0f;

				g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
				g_pStageSESpeaker->SetVolume(-pPuck->movex / MAX_PACK_SPEED);
			}
			else if (pPuck->CurPosx + pPuck->radius <= g_Stage.Left)
			{
				// �_�����Z
				AddScore_2P(pPuck->Score);
				// �G�t�F�N�g�̔���
				StartEffect(0);


				// �p�b�N������
				pPuck->exist = false;

				int P = 0;

				for (int u = 0; u < MAX_PUCK; u++)
				{
					// �p�b�N�̐����m�F
					if (pRootPuck[u].exist == true)
					{
						P++;
					}
				}
				if (P > 0)
				{
					continue;
				}

				// �}���b�g�̈ړ�
				MalletInfo *pMallet_1P;
				pMallet_1P = GetMallet_1P();
				pMallet_1P->State = MalletNonActive;
				float MalletRad = atan2f(MALLETDEFAULT_POSY_1P - pMallet_1P->CurPosy, MALLETDEFAULT_POSX_1P - pMallet_1P->CurPosx);
				pMallet_1P->movex = cosf(MalletRad)*15.0f;
				pMallet_1P->movey = sinf(MalletRad)*15.0f;

				// �}���b�g�̈ړ�
				MalletInfo *pMallet_2P;
				pMallet_2P = GetMallet_2P();
				pMallet_2P->State = MalletNonActive;
				MalletRad = atan2f(MALLETDEFAULT_POSY_2P - pMallet_2P->CurPosy, MALLETDEFAULT_POSX_2P - pMallet_2P->CurPosx);
				pMallet_2P->movex = cosf(MalletRad)*15.0f;
				pMallet_2P->movey = sinf(MalletRad)*15.0f;


				// �p�b�N�̐���
				CreatePuck((PuckKind)(rand() % MaxPuckKind), SCREEN_WIDTH / 2 - pPuck->radius * 2, 0);


			}
		}
		// �E�̕�
		if (pPuck->CurPosx + pPuck->radius >= g_Stage.Right)
		{
			// �S�[������
			if (GoalTop >= pPuck->CurPosy - pPuck->radius || GoalBottom <= pPuck->CurPosy + pPuck->radius)
			{
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx + pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(10);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(20);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(8);
					pEmitter->SetSpd((fabsf(pPuck->movey) * 0.5f)*0.5f);
					pEmitter->SetRadRange(PI / 2.0f, 3.0f*PI / 2.0f);
				}

				pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx + pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(15);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(20);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(5);
					pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.5);
					pEmitter->SetRadRange(PI / 2.0f, 3.0f*PI / 2.0f);
				}
				pPuck->CurPosx = g_Stage.Right - (pPuck->radius + 1.0f);
				pPuck->movex *= -1;

				g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
				g_pStageSESpeaker->SetVolume(pPuck->movex / MAX_PACK_SPEED);
			}
			else if (pPuck->CurPosx - pPuck->radius >= g_Stage.Right)
			{
				// ���_
				AddScore_1P(pPuck->Score);
				// �G�t�F�N�g�̔���
				StartEffect(1);


				// �p�b�N������
				pPuck->exist = false;

				int W = 0;


				for (int u = 0; u < MAX_PUCK; u++)
				{
					// �p�b�N�̐����m�F
					if (pRootPuck[u].exist == true)
					{
						W++;
					}
				}
				if (W > 0)
				{

					continue;

				}
				//�}���b�g�̈ʒu���f�t�H���g�ɖ߂�����
				MalletInfo *pMallet_1P;
				pMallet_1P = GetMallet_1P();
				pMallet_1P->State = MalletNonActive;
				float MalletRad = atan2f(MALLETDEFAULT_POSY_1P - pMallet_1P->CurPosy, MALLETDEFAULT_POSX_1P - pMallet_1P->CurPosx);
				pMallet_1P->movex = cosf(MalletRad)*15.0f;
				pMallet_1P->movey = sinf(MalletRad)*15.0f;

				// �}���b�g�̈ʒu�����ɖ߂�
				MalletInfo *pMallet_2P;
				pMallet_2P = GetMallet_2P();
				pMallet_2P->State = MalletNonActive;
				MalletRad = atan2f(MALLETDEFAULT_POSY_2P - pMallet_2P->CurPosy, MALLETDEFAULT_POSX_2P - pMallet_2P->CurPosx);
				pMallet_2P->movex = cosf(MalletRad)*15.0f;
				pMallet_2P->movey = sinf(MalletRad)*15.0f;

				// �p�b�N�̐���
				CreatePuck((PuckKind)(rand() % MaxPuckKind), SCREEN_WIDTH / 2 + pPuck->radius * 2, SCREEN_HEIGHT);

			}
		}
		// ��̕�
		if (pPuck->CurPosy - pPuck->radius < g_Stage.Top)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(5);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(60);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(10);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.35f)*0.9);
				pEmitter->SetRadRange(0, PI);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 0.5f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(45);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(12);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.7);
				pEmitter->SetRadRange(0, PI);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(35);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(14);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.15f)*0.6);
				pEmitter->SetRadRange(0, PI);
			}
			pPuck->CurPosy = g_Stage.Top + (pPuck->radius + 1.0f);
			pPuck->movey *= -1;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(pPuck->movey / MAX_PACK_SPEED);
		}
		// ���̕�
		if (pPuck->CurPosy + pPuck->radius > g_Stage.Bottom)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(5);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(60);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(10);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.35f)*0.9);
				pEmitter->SetRadRange(PI, PI * 2);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 0.5f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(45);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(12);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.7);
				pEmitter->SetRadRange(PI, PI * 2);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(35);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(12);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.15f)*0.6);
				pEmitter->SetRadRange(PI, PI * 2);
			}
			pPuck->CurPosy = g_Stage.Bottom - (pPuck->radius + 1.0f);
			pPuck->movey *= -1;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(pPuck->movey / MAX_PACK_SPEED);
		}


	}
}

void UpdateNormalOnlyStage()
{
	PuckInfo *pRootPuck = GetPuckInfo();
	PuckInfo *pPuck = pRootPuck;

	// �S�[���̔��˂���Ƃ���̏�̕�
	float GoalTop = (SCREEN_HEIGHT / 2.0f) - GOALHEIGHT / 2.0f;//(PUCKRADIUS + MALLETRADIUS / 2.0f);
	// �S�[���̔��˂���Ƃ���̉��̕�
	float GoalBottom = (SCREEN_HEIGHT / 2.0f) + GOALHEIGHT / 2.0f;//(PUCKRADIUS + MALLETRADIUS / 2.0f);

	for (int i = 0; i < MAX_PUCK; i++, pPuck++)
	{

		if (pPuck->exist == false)
		{
			continue;
		}

		// �ǂƂ̔���
		// ���̕�
		if (pPuck->CurPosx - pPuck->radius <= g_Stage.Left)
		{
			//	����������
			if (GoalTop >= pPuck->CurPosy - pPuck->radius || GoalBottom <= pPuck->CurPosy + pPuck->radius)
			{
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx - pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(10);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(10);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(8);
					pEmitter->SetSpd(fabsf(pPuck->movey) * 0.3f);
					pEmitter->SetRadRange(3.0f*PI / 2.0f, 5.0f*PI / 2.0f);
				}
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx - pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(15);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(10);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(5);
					pEmitter->SetSpd(fabsf(pPuck->movey) * 0.15f);
					pEmitter->SetRadRange(3.0f*PI / 2.0f, 5.0f*PI / 2.0f);
				}


				pPuck->CurPosx = g_Stage.Left + (pPuck->radius + 1.0f);
				pPuck->movex *= -1.0f;

				g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
				g_pStageSESpeaker->SetVolume(-pPuck->movex / MAX_PACK_SPEED);
			}
			// �S�[���ɓ������Ƃ�
			else if (pPuck->CurPosx + pPuck->radius <= g_Stage.Left)
			{
				// �_�����Z
				AddScore_2P(pPuck->Score);
				// �G�t�F�N�g�̔���
				StartEffect(0);


				// �p�b�N������
				pPuck->exist = false;

				int P = 0;

				for (int u = 0; u < MAX_PUCK; u++)
				{
					// �p�b�N�̐����m�F
					if (pRootPuck[u].exist == true)
					{
						P++;
					}
				}
				if (P > 0)
				{
					continue;
				}

				// �}���b�g�̈ړ�
				MalletInfo *pMallet_1P;
				pMallet_1P = GetMallet_1P();
				pMallet_1P->State = MalletNonActive;
				float MalletRad = atan2f(MALLETDEFAULT_POSY_1P - pMallet_1P->CurPosy, MALLETDEFAULT_POSX_1P - pMallet_1P->CurPosx);
				pMallet_1P->movex = cosf(MalletRad)*15.0f;
				pMallet_1P->movey = sinf(MalletRad)*15.0f;

				// �}���b�g�̈ړ�
				MalletInfo *pMallet_2P;
				pMallet_2P = GetMallet_2P();
				pMallet_2P->State = MalletNonActive;
				MalletRad = atan2f(MALLETDEFAULT_POSY_2P - pMallet_2P->CurPosy, MALLETDEFAULT_POSX_2P - pMallet_2P->CurPosx);
				pMallet_2P->movex = cosf(MalletRad)*15.0f;
				pMallet_2P->movey = sinf(MalletRad)*15.0f;


				// �p�b�N�̐���
				CreatePuck(NormalPuck, SCREEN_WIDTH / 2 - pPuck->radius * 2, 0);


			}
		}
		// �E�̕�
		if (pPuck->CurPosx + pPuck->radius >= g_Stage.Right)
		{
			// �S�[������
			if (GoalTop >= pPuck->CurPosy - pPuck->radius || GoalBottom <= pPuck->CurPosy + pPuck->radius)
			{
				// �p�[�e�B�N���G�t�F�N�g�̍쐬
				ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx + pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(10);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(20);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(8);
					pEmitter->SetSpd((fabsf(pPuck->movey) * 0.5f)*0.5f);
					pEmitter->SetRadRange(PI / 2.0f, 3.0f*PI / 2.0f);
				}

				pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
				if (pEmitter != nullptr)
				{
					Float2 Pos(pPuck->CurPosx + pPuck->radius, pPuck->CurPosy);
					Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
					pEmitter->CreateParticle(15);
					pEmitter->SetColor(Color);
					pEmitter->SetLife(20);
					pEmitter->SetPos(Pos);
					pEmitter->SetSize(5);
					pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.5);
					pEmitter->SetRadRange(PI / 2.0f, 3.0f*PI / 2.0f);
				}
				pPuck->CurPosx = g_Stage.Right - (pPuck->radius + 1.0f);
				pPuck->movex *= -1;

				g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
				g_pStageSESpeaker->SetVolume(pPuck->movex / MAX_PACK_SPEED);
			}
			else if (pPuck->CurPosx - pPuck->radius >= g_Stage.Right)
			{
				// ���_
				AddScore_1P(pPuck->Score);
				// �G�t�F�N�g�̔���
				StartEffect(1);


				// �p�b�N������
				pPuck->exist = false;

				int W = 0;


				for (int u = 0; u < MAX_PUCK; u++)
				{
					// �p�b�N�̐����m�F
					if (pRootPuck[u].exist == true)
					{
						W++;
					}
				}
				if (W > 0)
				{

					continue;

				}
				//�}���b�g�̈ʒu���f�t�H���g�ɖ߂�����
				MalletInfo *pMallet_1P;
				pMallet_1P = GetMallet_1P();
				pMallet_1P->State = MalletNonActive;
				float MalletRad = atan2f(MALLETDEFAULT_POSY_1P - pMallet_1P->CurPosy, MALLETDEFAULT_POSX_1P - pMallet_1P->CurPosx);
				pMallet_1P->movex = cosf(MalletRad)*15.0f;
				pMallet_1P->movey = sinf(MalletRad)*15.0f;

				// �}���b�g�̈ʒu�����ɖ߂�
				MalletInfo *pMallet_2P;
				pMallet_2P = GetMallet_2P();
				pMallet_2P->State = MalletNonActive;
				MalletRad = atan2f(MALLETDEFAULT_POSY_2P - pMallet_2P->CurPosy, MALLETDEFAULT_POSX_2P - pMallet_2P->CurPosx);
				pMallet_2P->movex = cosf(MalletRad)*15.0f;
				pMallet_2P->movey = sinf(MalletRad)*15.0f;

				// �p�b�N�̐���
				CreatePuck(NormalPuck, SCREEN_WIDTH / 2 + pPuck->radius * 2, SCREEN_HEIGHT);

			}
		}
		// ��̕�
		if (pPuck->CurPosy - pPuck->radius < g_Stage.Top)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(5);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(60);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(10);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.35f)*0.9);
				pEmitter->SetRadRange(0, PI);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 0.5f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(45);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(12);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.7);
				pEmitter->SetRadRange(0, PI);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy - pPuck->radius);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(35);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(14);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.15f)*0.6);
				pEmitter->SetRadRange(0, PI);
			}
			pPuck->CurPosy = g_Stage.Top + (pPuck->radius + 1.0f);
			pPuck->movey *= -1;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(pPuck->movey / MAX_PACK_SPEED);
		}
		// ���̕�
		if (pPuck->CurPosy + pPuck->radius > g_Stage.Bottom)
		{
			// �p�[�e�B�N���G�t�F�N�g�̍쐬
			ParticleEmitter *pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 0.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(5);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(60);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(10);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.35f)*0.9);
				pEmitter->SetRadRange(PI, PI * 2);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 0.5f, 0.0f, 1.0f);
				pEmitter->CreateParticle(10);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(45);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(12);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.25f)*0.7);
				pEmitter->SetRadRange(PI, PI * 2);
			}

			pEmitter = GetScene()->GetEmitMngr()->GetFreeEmitter();
			if (pEmitter != nullptr)
			{
				Float2 Pos(pPuck->CurPosx, pPuck->CurPosy + pPuck->radius);
				Float4 Color(1.0f, 1.0f, 0.0f, 1.0f);
				pEmitter->CreateParticle(15);
				pEmitter->SetColor(Color);
				pEmitter->SetLife(35);
				pEmitter->SetPos(Pos);
				pEmitter->SetSize(12);
				pEmitter->SetSpd((fabsf(pPuck->movey) * 0.15f)*0.6);
				pEmitter->SetRadRange(PI, PI * 2);
			}

			pPuck->CurPosy = g_Stage.Bottom - (pPuck->radius + 1.0f);
			pPuck->movey *= -1;

			g_pStageSESpeaker = PlaySound(GetSoundEffect(SE_HIT_WALL_PUCK));
			g_pStageSESpeaker->SetVolume(pPuck->movey / MAX_PACK_SPEED);
		}


	}
}
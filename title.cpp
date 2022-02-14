#include "title.h"
#include "Texture.h"
#include "Keyboard.h"
#include "SceneManager.h"
#include "game.h"
#include "myPolygon.h"
#include "Source.h"
#include "JoyPad.h"
#include <math.h>
#include "Stage.h"
#include "Puck.h"
#include "Sound.h"


#define PI (3.14159265359)
#define PushAWidth (700.0f)
#define PushAHeight (400.0f)
#define ANIM_SCORE_SPLIT_X (10)  // �摜�̕�����(��)
#define ANIM_SCORE_SPLIT_Y (1)  // �摜�̕�����(�c)
#define MAX_POINT (24) // �Q�[���̍ō����_
#define KEY_DELAY	(15)

// �\����
typedef struct 
{
	float Posx;
	float Posy;
	float width;
	float height;
	float Alpha;
	int count;
	int SplitU;
	int SplitV;
	int TexIdx;
	int Num;
	int MaxNum;
	ID3D11ShaderResourceView *pTexture;

}TexturePolygon;

enum MenuKind
{
	PointMenu,
	StageMenu,
	//ItemMenu,

	MaxMenu
};

// �O���[�o���ϐ�
//ID3D11ShaderResourceView *g_pTitleTex;
TexturePolygon g_PushA;
ID3D11ShaderResourceView *g_pNumberTex;
//                    �s      ��
TexturePolygon g_menu[MaxMenu][2];
bool TitleSwitchFlag;
TexturePolygon g_StartButton;
TexturePolygon g_MainRule;
int SelctColumn; // �s
ID3D11ShaderResourceView *g_TitleLogo;
XAUDIO2_BUFFER *g_pTitleBGM;
IXAudio2SourceVoice *g_pTitleBGMSpeaker;


HRESULT InitTitle()
{
	SelctColumn = 0;
	g_PushA.count = 0;
	TitleSwitchFlag = false;
	// �e�N�X�`���ǂݍ���
	HRESULT hr;

	hr = LoadTextureFromFile("menu/malesuma.png", &g_TitleLogo);
	if (FAILED(hr))
	{
		return hr;
	}

	hr= LoadTextureFromFile("menu/titlerogo.png", &g_PushA.pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	hr= LoadTextureFromFile("image/number.png", &g_pNumberTex);
	if (FAILED(hr))
	{
		return hr;
	}

	hr= LoadTextureFromFile("menu/Point.png", &g_menu[PointMenu][0].pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = LoadTextureFromFile("menu/Stage.png", &g_menu[StageMenu][0].pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	//hr = LoadTextureFromFile("menu/Item.png", &g_menu[ItemMenu][0].pTexture);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}

	hr = LoadTextureFromFile("menu/StartGame.png",&g_StartButton.pTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = LoadTextureFromFile("menu/MainRule.png", &g_MainRule.pTexture);
	if (FAILED(hr))
	{
		return hr;
	}

	// �e�N�X�`���ݒ�
	g_menu[0][1].pTexture = g_pNumberTex;
	g_menu[1][1].pTexture = g_pNumberTex;


	// YesNo�̉摜�������Ă���
	//hr = LoadTextureFromFile("menu/YesNo.png", &g_menu[ItemMenu][1].pTexture);
	//if (FAILED(hr))
	//{
	//	return hr;
	//}

	// Push�̏�����
	g_PushA.width = PushAWidth;
	g_PushA.height = PushAHeight;
	g_PushA.Posx = SCREEN_WIDTH / 2.0f;
	g_PushA.Posy = SCREEN_HEIGHT - g_PushA.height/4.0f;
	g_PushA.Alpha = 1.0f;

	float fBasePosX = SCREEN_WIDTH * 0.5f;
	float fBasePosY = SCREEN_HEIGHT * 0.5f;

	// ���j���[�̏�����
	for (int i = 0; i < MaxMenu; i++)
	{
		g_menu[i][0].width = 300.0f;
		g_menu[i][0].height = 100.0f;
		g_menu[i][0].Posx = fBasePosX - (g_menu[i][0].width *0.5f);
		g_menu[i][0].Posy = fBasePosY - ((MaxMenu - 1)*0.5f - i)*g_menu[i][0].height*1.5f;
		g_menu[i][0].SplitU = 1;
		g_menu[i][0].SplitV = 1;
		g_menu[i][0].TexIdx = 0;
		g_menu[i][0].Num = 0;
		g_menu[i][0].MaxNum = 0;

		g_menu[i][1].width = 100.0f;
		g_menu[i][1].height = 100.0f;
		g_menu[i][1].Posx = fBasePosX + (g_menu[i][0].width);
		g_menu[i][1].Posy = g_menu[i][0].Posy;
		g_menu[i][1].SplitU = ANIM_SCORE_SPLIT_X;
		g_menu[i][1].SplitV = ANIM_SCORE_SPLIT_Y;
		g_menu[i][1].TexIdx = 0;
		g_menu[i][1].Num = 0;
	}

	// �A�C�e�����ڂ�UV�ݒ�
	//g_menu[ItemMenu][1].SplitU = 2;
	//g_menu[ItemMenu][1].SplitV = 1;


	// �e���ڂ̏��
	g_menu[PointMenu][1].MaxNum = MAX_POINT;
	g_menu[StageMenu][1].MaxNum = (int)MaxStage;
	//g_menu[ItemuMenu][1].MaxNum = 2;

	// �X�^�[�g�{�^�����ڂ̏�����
	g_StartButton.Posx = SCREEN_WIDTH / 2;
	g_StartButton.Posy = g_menu[MaxMenu - 1][0].Posy + g_menu[MaxMenu - 1][0].height;
	g_StartButton.width = 300.0f;
	g_StartButton.height = 100.0f;

	// ���C�����[�����ڂ̏�����
	g_MainRule.width = 300.0f;
	g_MainRule.height = 100.0f;
	g_MainRule.Posx = SCREEN_WIDTH / 2;
	g_MainRule.Posy = g_menu[0][0].Posy - g_menu[0][0].height;

	// �����ڕW�X�R�A
	g_menu[PointMenu][1].Num = 12 - 1;

	// BGM
	//g_pTitleBGM = CreateSound("Sound / title.wav", true);
	//g_pTitleBGMSpeaker = PlaySound(g_pTitleBGM);
	//g_pTitleBGMSpeaker->SetVolume(1.0f);

	CreateMaxPuck();

	return hr;
}
void UninitTitle()
{
	SAFE_RELEASE(g_TitleLogo);
	SAFE_RELEASE(g_PushA.pTexture);
	SAFE_RELEASE(g_pNumberTex);
	SAFE_RELEASE(g_StartButton.pTexture);
	SAFE_RELEASE(g_MainRule.pTexture);
	for (int i = 0; i < MaxMenu; i++)
	{
		SAFE_RELEASE(g_menu[i][0].pTexture);
	}
	//SAFE_RELEASE(g_menu[ItemMenu][1].pTexture);

	//g_pTitleBGMSpeaker->Stop();
}
void UpdateTitle()
{
	XINPUT_STATE PadState = GetGamePadState(0);
	static int keyDelay = 0;

	if (TitleSwitchFlag == false)
	{
		

		if (IsKeyTrigger(VK_RETURN) || PadState.Gamepad.wButtons&XINPUT_GAMEPAD_B)
		{
			// �V�[���ړ��@����V�[�����ړ��������������炱����g���@GetScene()->SetNextScene(���������Ƃ���);
			//GetScene()->SetNextScene(Scene_Game);

			TitleSwitchFlag = true;
		}

		// �p�x�𑝂₷�iAlpha�ׁ̈j
		g_PushA.count += 2;
		g_PushA.Alpha = sin(g_PushA.count * PI / 180)*0.4f + 0.6f;

	}
	else
	{
		//static int SelctRow = 0; // ��
		//static int SelctColumn = 0; // �s

		// �ڎ��؂�ւ�
		// �\���L�[��
		if (PadState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN || IsKeyPress(VK_DOWN))
		{
			if (keyDelay < 0)
			{
				SelctColumn = (SelctColumn + 1) % (MaxMenu + 1);
				keyDelay = KEY_DELAY;
			}
		}
		// �\���L�[��
		else if (PadState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP || IsKeyPress(VK_UP))
		{
			if (keyDelay < 0)
			{
				SelctColumn = (SelctColumn + MaxMenu) % (MaxMenu + 1);
				keyDelay = KEY_DELAY;
			}
		}
		// ���[���؂�ւ�
		// �\���L�[��
		else if (PadState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT || IsKeyPress(VK_LEFT))
		{
			if (keyDelay < 0 && SelctColumn < 3)
			{
				g_menu[SelctColumn][1].Num = (g_menu[SelctColumn][1].Num + g_menu[SelctColumn][1].MaxNum - 1) % g_menu[SelctColumn][1].MaxNum;
				keyDelay = KEY_DELAY;
			}
		}
		// �\���L�[�E
		else if (PadState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT || IsKeyPress(VK_RIGHT))
		{
			if (keyDelay < 0 && SelctColumn < 3)
			{
				g_menu[SelctColumn][1].Num = (g_menu[SelctColumn][1].Num + 1) % g_menu[SelctColumn][1].MaxNum;
				keyDelay = KEY_DELAY;
			}
		}
		else
		{
			keyDelay = 0;
		}

		if (SelctColumn == MaxMenu)
		{

			if (IsKeyTrigger(VK_RETURN) || PadState.Gamepad.wButtons&XINPUT_GAMEPAD_B)
			{
				// �V�[���ړ��@����V�[�����ړ��������������炱����g���@GetScene()->SetNextScene(���������Ƃ���);
				GetScene()->SetNextScene(Scene_Game);
			}
		}

	}
	keyDelay--;
}
void DrawTitle()
{
	SetPolygonSplitUV(0, 1, 1);

	//// �w�i�G
	//SetPolygonPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	//SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//SetPolygonVtxColor(0, 1, 1, 1, 1);
	//SetPolygonVtxColor(1, 1, 1, 1, 1);
	//SetPolygonVtxColor(2, 1, 1, 1, 1);
	//SetPolygonVtxColor(3, 1, 1, 1, 1);
	////SetPolygonTexture(g_pTitleTex);

	//DrawPolygon();

	if (TitleSwitchFlag == false)
	{
		// �^�C�g�����S
		SetPolygonPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT/6);
		SetPolygonSize(SCREEN_WIDTH/2,SCREEN_HEIGHT/4);
		SetPolygonVtxColor(0, 0, 0, 0, 1);
		SetPolygonVtxColor(1, 0, 0, 0, 1);
		SetPolygonVtxColor(2, 0, 0, 0, 1);
		SetPolygonVtxColor(3, 0, 0, 0, 1);
		SetPolygonTexture(g_TitleLogo);
		DrawPolygon();
		// �{�^�������Ă̂�� Push B Button
		SetPolygonPos(g_PushA.Posx, g_PushA.Posy);
		SetPolygonSize(g_PushA.width, g_PushA.height);
		SetPolygonVtxColor(0, 1, 0, 0, g_PushA.Alpha);
		SetPolygonVtxColor(1, 1, 0, 0, g_PushA.Alpha);
		SetPolygonVtxColor(2, 1, 0, 0, g_PushA.Alpha);
		SetPolygonVtxColor(3, 1, 0, 0, g_PushA.Alpha);
		SetPolygonTexture(g_PushA.pTexture);
		DrawPolygon();

	}
	else
	{

		// ���j���[�����₷������|���S��
		SetPolygonPos((g_menu[0][0].Posx - g_menu[0][0].width / 2 + g_menu[0][1].Posx + g_menu[0][1].width / 2) / 2,
			(g_menu[0][0].Posy - g_menu[0][0].height / 2 + g_menu[MaxMenu - 1][0].Posy + g_menu[MaxMenu - 1][0].height / 2) / 2);
		SetPolygonSize((g_menu[0][1].Posx+g_menu[0][1].width/2) - (g_menu[0][0].Posx - g_menu[0][0].width / 2) + 20.0f, g_menu[0][0].height *5 + 10.0f);
		SetPolygonVtxColor(0, 0, 0, 0, 0.8f);
		SetPolygonVtxColor(1, 0, 0, 0, 0.8f);
		SetPolygonVtxColor(2, 0, 0, 0, 0.8f);
		SetPolygonVtxColor(3, 0, 0, 0, 0.8f);
		SetPolygonTexture(NULL);

		DrawPolygon();

		// �Q�[���X�^�[�g
		SetPolygonPos(g_StartButton.Posx, g_StartButton.Posy);
		SetPolygonSize(g_StartButton.width, g_StartButton.height);
		SetPolygonVtxColor(0, 1, 1, 1, 1);
		SetPolygonVtxColor(1, 1, 1, 1, 1);
		SetPolygonVtxColor(2, 1, 1, 1, 1);
		SetPolygonVtxColor(3, 1, 1, 1, 1);
		SetPolygonTexture(g_StartButton.pTexture);
		DrawPolygon();

		// ���C�����[��
		SetPolygonPos(g_MainRule.Posx, g_MainRule.Posy);
		SetPolygonSize(g_MainRule.width, g_MainRule.height);
		SetPolygonVtxColor(0, 1, 1, 1, 1);
		SetPolygonVtxColor(1, 1, 1, 1, 1);
		SetPolygonVtxColor(2, 1, 1, 1, 1);
		SetPolygonVtxColor(3, 1, 1, 1, 1);
		SetPolygonTexture(g_MainRule.pTexture);
		DrawPolygon();

		// ���j���[�e����
		for (int i = 0; i < MaxMenu; i++)
		{
			for (int j = 0; j < 2; j++)
			{

				SetPolygonTexture(g_menu[i][j].pTexture);

				int work = g_menu[i][j].Num + 1;
				int x = 0;
				do
				{

				SetPolygonPos(g_menu[i][j].Posx-x*g_menu[i][j].width, g_menu[i][j].Posy);
				SetPolygonSize(g_menu[i][j].width, g_menu[i][j].height);
				SetPolygonVtxColor(0, 1, 1, 1, 1);
				SetPolygonVtxColor(1, 1, 1, 1, 1);
				SetPolygonVtxColor(2, 1, 1, 1, 1);
				SetPolygonVtxColor(3, 1, 1, 1, 1);
				SetPolygonSplitUV(work%10, g_menu[i][j].SplitU, g_menu[i][j].SplitV);
				work /= 10;
				DrawPolygon();
				x++;
				} while (work > 0);

			}
		}
		if (SelctColumn < MaxMenu)
		{
			SetPolygonVtxColor(0, 1, 0, 0, 0.4);
			SetPolygonVtxColor(1, 1, 0, 0, 0.4);
			SetPolygonVtxColor(2, 1, 0, 0, 0.4);
			SetPolygonVtxColor(3, 1, 0, 0, 0.4);
			SetPolygonTexture(NULL);
			
			int work = g_menu[SelctColumn][1].Num + 1;
			int x = 0;
			do
			{
				work /= 10;


				SetPolygonPos(g_menu[SelctColumn][1].Posx-x*g_menu[SelctColumn][1].width, g_menu[SelctColumn][1].Posy);
				SetPolygonSize(g_menu[SelctColumn][1].width, g_menu[SelctColumn][1].height);
				DrawPolygon();
				x++;
			} while (work > 0);


		}
		else
		{
			SetPolygonPos(g_StartButton.Posx, g_StartButton.Posy);
			SetPolygonSize(g_StartButton.width, g_StartButton.height);
			SetPolygonVtxColor(0, 1, 0, 0, 0.4);
			SetPolygonVtxColor(1, 1, 0, 0, 0.4);
			SetPolygonVtxColor(2, 1, 0, 0, 0.4);
			SetPolygonVtxColor(3, 1, 0, 0, 0.4);
			SetPolygonTexture(NULL);
			DrawPolygon();
		}

	}


}
int GetMaxScore()
{
	return g_menu[PointMenu][1].Num;
}
StageKind GetSelectStage()
{
	return (StageKind)g_menu[StageMenu][1].Num;
}
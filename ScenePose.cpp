#include "ScenePose.h"
#include "CMyPolygon.h"
#include "Game.h"
#include "Keyboard.h"
#include "SceneManager.h"
#include "DirectX.h"
#include "Source.h"
#include "JoyPad.h"

#define KEY_DELAY	(15)

HRESULT	ScenePose::Init()
{
	HRESULT hr = S_OK;

	hr = LoadTextureFromFile("menu/Pose_continue.png", &m_pTex[POSE_CONTINUE]);
	if (FAILED(hr))
		return hr;

	hr = LoadTextureFromFile("menu/Pose_exit.png", &m_pTex[POSE_EXIT]);
	if (FAILED(hr))
		return hr;

	m_bOpen = false;
	m_bTitleBack = false;
	m_SelectCursor = POSE_CONTINUE;
	m_fMagnification = 0.0f;

	return hr;
}


void ScenePose::Uninit()
{
	for (int i = 0; i < POSE_MAX; i++)
		SAFE_RELEASE(m_pTex[i]);
}


void ScenePose::Update()
{
	XINPUT_STATE PadState = GetGamePadState(0);
	static int keyDelay = 0;


	// ポーズ開閉
	if (IsKeyTrigger('M') || PadState.Gamepad.wButtons&XINPUT_GAMEPAD_START)
	{
		if (keyDelay < 0)
		{
			m_bOpen = !m_bOpen;
			//m_bOpen ^= true;
			m_SelectCursor = POSE_CONTINUE;
			m_fMagnification = 0.0f;
			keyDelay = KEY_DELAY;
		}
	}

	if (m_bOpen)
	{
		if (m_fMagnification < 1.0f)
			m_fMagnification += 0.05f;

		if (IsKeyRepeat(VK_UP) || PadState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP)
		{
			if (keyDelay < 0)
			{
				m_SelectCursor = (POSE_SELECT)((m_SelectCursor + POSE_MAX - 1) % (POSE_MAX));
				int i = 0;
				keyDelay = KEY_DELAY;
			}
		}

		if (IsKeyRepeat(VK_DOWN) || PadState.Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN)
		{
			if (keyDelay < 0)
			{
				m_SelectCursor = (POSE_SELECT)((m_SelectCursor + 1) % (POSE_MAX));
				int i = 0;
				keyDelay = KEY_DELAY;
			}
		}

		if (IsKeyTrigger(VK_RETURN) || PadState.Gamepad.wButtons&XINPUT_GAMEPAD_B)
		{
			switch (m_SelectCursor)
			{
			case POSE_CONTINUE:
				m_bOpen = false;	// ポーズを閉じる
				break;

			case POSE_EXIT:
				m_bTitleBack = true;
				//GetSceneMngr()->SetNextScene(SCENE_TITLE);	// タイトル画面へ
				break;

			default:
				break;
			}
		}
	}
	keyDelay--;
}


void ScenePose::Draw()
{
	if (!m_bOpen)
		return;


	CMyPolygon Polygon;

	// 黒背景
	SetPolygonTexture(NULL);
	Polygon.SetPos(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	Polygon.SetSize(SCREEN_WIDTH * m_fMagnification, SCREEN_HEIGHT * m_fMagnification);
	Polygon.SetColor(0.0f, 0.0f, 0.0f, 0.5f);
	Polygon.Draw();

	// 選択カーソル
	Polygon.SetColor(1.0f, 1.0f, 1.0f, 0.7f);
	Polygon.SetSize(SCREEN_WIDTH / 3.0f * m_fMagnification, SCREEN_HEIGHT / 7.0f * m_fMagnification);
	switch (m_SelectCursor)
	{
	case POSE_CONTINUE:
		Polygon.SetPos(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f *(1.0f - m_fMagnification) + SCREEN_HEIGHT / 3.0f * m_fMagnification);
		break;

	case POSE_EXIT:
		Polygon.SetPos(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f *(1.0f - m_fMagnification) + SCREEN_HEIGHT * 2.0f / 3.0f * m_fMagnification);
		break;

	default:
		break;
	}
	Polygon.Draw();
	

	Polygon.SetColor(1.0f, 1.0f, 0.0f, 1.0f);
	// つづける
	SetPolygonTexture(m_pTex[POSE_CONTINUE]);
	Polygon.SetPos(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f *(1.0f - m_fMagnification) + SCREEN_HEIGHT / 3.0f * m_fMagnification);
	Polygon.SetSplitUV(0, 1, 1);
	Polygon.Draw();
	// やめる
	SetPolygonTexture(m_pTex[POSE_EXIT]);
	Polygon.SetPos(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f *(1.0f - m_fMagnification) + SCREEN_HEIGHT * 2.0f / 3.0f * m_fMagnification);
	Polygon.Draw();

}


bool ScenePose::IsActive()
{
	return m_bOpen;
}


bool ScenePose::IsTitleBack()
{
	return m_bTitleBack;
}
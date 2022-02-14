#include "SceneManager.h"
#include "title.h"
#include "mallet.h"
#include "Puck.h"
#include "score.h"
#include "goal.h"
#include "Effect.h"
#include "Stage.h"

#define MAX_PARTICLE_EMITTER		(200)

HRESULT SceneManager:: Init(HWND hWnd)
{
	m_hWnd = hWnd;
	HRESULT hr = S_OK;

	m_isPlayingVideo = false;

	// フェードの初期化
	hr = m_Fade.Init();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "fadeの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	this->m_nStopCnt = 0;

	switch (m_CurScene)
	{
	case Scene_Title:
		// バックグラウンドの初期化
		hr = InitStage(TutorialStage);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Bgの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// マレットの初期化
		hr = InitMallet();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Malletの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// パックの初期化
		hr = InitPuck();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Puckの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// タイトルの初期化
		hr=InitTitle();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "タイトルの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// パーティクルエフェクトの初期化
		m_pEmitMngr = new EmitterMngr;
		m_pEmitMngr->Init(50);
	
		break;
	case Scene_Game:
		// バックグラウンドの初期化
		hr = InitStage(GetSelectStage());
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Bgの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		hr = InitScore();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Scoreの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// マレットの初期化
		hr = InitMallet();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Malletの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// パックの初期化
		hr = InitPuck();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Puckの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// ゴールの初期化
		InitGoal();

		hr = m_pose.Init();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Poseの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}

		// エフェクトの初期化
		InitEffect();
		// パーティクルエフェクトの初期化
		m_pEmitMngr = new EmitterMngr;
		m_pEmitMngr->Init(MAX_PARTICLE_EMITTER);

		break;
	default:
		break;
	}

	return hr;
}
void SceneManager::Uninit()
{
	// 
	m_Fade.Uninit();
	switch (m_CurScene)
	{
	case Scene_Title:
		// パーティクルエフェクトの終了
		m_pEmitMngr->UnInit();
		delete m_pEmitMngr;
		UninitTitle();
		// パックの終了
		UninitPuck();
		// マレットの終了
		UninitMallet();
		// バックグラウンドの終了
		UninitStage();
		break;
	case Scene_Game:
		// パーティクルエフェクトの終了
		m_pEmitMngr->UnInit();
		delete m_pEmitMngr;
		// エフェクトの終了
		UninitEffect();
		// ポーズの終了
		m_pose.Uninit();
		// ゴールの終了
		UninitGoal();
		// パックの終了
		UninitPuck();
		// マレットの終了
		UninitMallet();
		// スコアの終了
		UninitScore();
		// バックグラウンドの終了
		UninitStage();

		break;
	default:
		break;
	}
}
void SceneManager::Update()
{
	// いつでも使えるようにしておく
	// フェード
	m_Fade.Update();

	// シーン切り替えで必要なものを初期化する
	if (m_NextScene != Max_Scene)
	{

		if (m_Fade.IsFinsh() == true)
		{
			Uninit();
			m_CurScene = m_NextScene;
			m_NextScene = Max_Scene;
			Init(m_hWnd);
			m_Fade.Start(1.0f, false);
		}
	}


	switch (m_CurScene)
	{
	case Scene_Title:
		// タイトルの更新
		UpdateTitle();
		// マレットの更新
		UpdateMallet();
		// パックの更新
		UpdatePuck();
		// ステージの更新
		UpdateStage();
		// パーティクルエフェクトの更新
		m_pEmitMngr->Update();

		break;
	case Scene_Game:
		m_pose.Update();

		if (!m_pose.IsActive())		// ポーズ
		{
			if (m_nStopCnt < 1)			// ヒットストップ
			{
				// マレットの更新
				UpdateMallet();
				// パックの更新
				UpdatePuck();
				// ステージの更新
				UpdateStage();
				// スコアの更新
				UpdateScore();
				// エフェクトの更新
				UpdateEffect();
				// ゴールの更新
				UpdateGoal();
			}
			else
			{
				m_nStopCnt--;
			}
			// パーティクルエフェクトの更新
			m_pEmitMngr->Update();
		}
		else
		{
			if (m_pose.IsTitleBack())
				SetNextScene(Scene_Title);
		}

		break;
	default:
		break;
	}
}
// 今のシーンで必要なものを描画する
void SceneManager::Draw()
{

	switch (m_CurScene)
	{
	case Scene_Title:
		// バックグラウンドの描画
		DrawStage();
		// マレットの描画
		DrawMallet();
		// パックの描画
		DrawPuck();
		// タイトルの描画
		DrawTitle();
		// パーティクルエフェクトの描画
		m_pEmitMngr->Draw();
		break;
	case Scene_Game:
		// バックグラウンドの描画
		DrawStage();
		// スコアの描画
		DrawScore();
		// ゴールの描画
		DrawGoal();
		// エフェクトの描画
		DrawEffect();
		// マレットの描画
		DrawMallet();
		// パックの描画
		DrawPuck();
		// パーティクルエフェクトの描画
		m_pEmitMngr->Draw();
		// ポーズの描画
		m_pose.Draw();

		break;
	default:
		break;
	}
	// フェードの描画
	m_Fade.Draw();
}
// もらったシーンを設定する
void SceneManager::SetStartScene()
{
	m_CurScene = Scene_Title;
	m_NextScene = Max_Scene;
}
// game.cpから今のシーンを受け取る
void SceneManager::SetNextScene(SCENE_KIND NextScene)
{
	
	m_NextScene = NextScene;
	if (m_Fade.IsExecute() == false)
	{
		m_Fade.Start(1.0f, true);
	}
}

Fade SceneManager::GetFade()
{
	return m_Fade;
}

EmitterMngr* SceneManager::GetEmitMngr()
{
	return m_pEmitMngr;
}


void SceneManager::SetStopCnt(int nStopCnt)
{
	this->m_nStopCnt = nStopCnt;
}


// 動画再生中かどうか
void SceneManager::IsPlayingVideo(bool bPlaying)
{
	m_isPlayingVideo = bPlaying;
}


bool SceneManager::IsPlayingVideo()
{
	return m_isPlayingVideo;
}

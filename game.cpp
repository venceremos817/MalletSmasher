#include "game.h"
#include "DirectX.h"
#include "Polygon.h"
#include "Keyboard.h"
#include "SceneManager.h"
#include "JoyPad.h"
#include "Sound.h"
#include "score.h"
#include "SoundEffect.h"

// ぐろばる
SceneManager *g_pSceneManager;
JoyPad *g_pJoyPad;



HRESULT Init(HWND hWnd, UINT widht, UINT height)
{
	HRESULT hr = S_OK;
	// システム面の初期化 falseにしておくこと！！！！！！！！！！！！！！！
	hr = InitDirextX(hWnd, widht, height, false);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "DirectXの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	hr = InitSound();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "音の初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	//〜〜〜〜〜〜〜〜〜〜〜〜
	InitSoundEffect();
	InitBGM();
	//〜〜〜〜〜〜〜〜〜〜〜〜

	//----- ゲーム内のオブジェクトの初期化
	// ポリゴンの初期化
	hr = InitPolygon(widht, height);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Polygonの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// キーボードの初期化
	hr = InitKeyboard();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Keyboardの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	
	// ジョイパッド初期化
	g_pJoyPad = new JoyPad;
	hr = g_pJoyPad->Init();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ゲームパッドの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// シーンの初期化
	g_pSceneManager = new SceneManager;
	// 初めのシーン決め
	g_pSceneManager->SetStartScene();
	hr = g_pSceneManager->Init(hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Sceneの初期化に失敗しました", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}


	

	return hr;
}
void Uninit()
{
	//〜〜〜〜〜〜〜〜〜〜〜〜
	UninitSoundEffect();
	UninitBGM();
	//〜〜〜〜〜〜〜〜〜〜〜〜
	g_pSceneManager->Uninit();
	delete g_pSceneManager;

	// ジョイパッドの終了
	delete g_pJoyPad;
	UninitKeyboard();
	UninitPolygon();
	UninitSound();
	UninitDirectX();
}
void Update()
{
	UpdateKeyboard();
	// ジョイパッドの更新
	g_pJoyPad->Update();

	g_pSceneManager->Update();
}
void Draw()
{
	// 動画再生中でなければポリゴン描画
	if (!g_pSceneManager->IsPlayingVideo())
	{
		BeginDrawDirectX();

		g_pSceneManager->Draw();

		EndDrawDirectX();
	}
}
SceneManager *GetScene()
{
	return g_pSceneManager;
}
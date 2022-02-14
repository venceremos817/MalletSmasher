#include "game.h"
#include "DirectX.h"
#include "Polygon.h"
#include "Keyboard.h"
#include "SceneManager.h"
#include "JoyPad.h"
#include "Sound.h"
#include "score.h"
#include "SoundEffect.h"

// ‚®‚ë‚Î‚é
SceneManager *g_pSceneManager;
JoyPad *g_pJoyPad;



HRESULT Init(HWND hWnd, UINT widht, UINT height)
{
	HRESULT hr = S_OK;
	// ƒVƒXƒeƒ€–Ê‚Ì‰Šú‰» false‚É‚µ‚Ä‚¨‚­‚±‚ÆIIIIIIIIIIIIIII
	hr = InitDirextX(hWnd, widht, height, false);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "DirectX‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	hr = InitSound();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "‰¹‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	//````````````
	InitSoundEffect();
	InitBGM();
	//````````````

	//----- ƒQ[ƒ€“à‚ÌƒIƒuƒWƒFƒNƒg‚Ì‰Šú‰»
	// ƒ|ƒŠƒSƒ“‚Ì‰Šú‰»
	hr = InitPolygon(widht, height);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Polygon‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// ƒL[ƒ{[ƒh‚Ì‰Šú‰»
	hr = InitKeyboard();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Keyboard‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	
	// ƒWƒ‡ƒCƒpƒbƒh‰Šú‰»
	g_pJoyPad = new JoyPad;
	hr = g_pJoyPad->Init();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "ƒQ[ƒ€ƒpƒbƒh‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// ƒV[ƒ“‚Ì‰Šú‰»
	g_pSceneManager = new SceneManager;
	// ‰‚ß‚ÌƒV[ƒ“Œˆ‚ß
	g_pSceneManager->SetStartScene();
	hr = g_pSceneManager->Init(hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Scene‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}


	

	return hr;
}
void Uninit()
{
	//````````````
	UninitSoundEffect();
	UninitBGM();
	//````````````
	g_pSceneManager->Uninit();
	delete g_pSceneManager;

	// ƒWƒ‡ƒCƒpƒbƒh‚ÌI—¹
	delete g_pJoyPad;
	UninitKeyboard();
	UninitPolygon();
	UninitSound();
	UninitDirectX();
}
void Update()
{
	UpdateKeyboard();
	// ƒWƒ‡ƒCƒpƒbƒh‚ÌXV
	g_pJoyPad->Update();

	g_pSceneManager->Update();
}
void Draw()
{
	// “®‰æÄ¶’†‚Å‚È‚¯‚ê‚Îƒ|ƒŠƒSƒ“•`‰æ
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
#include "game.h"
#include "DirectX.h"
#include "Polygon.h"
#include "Keyboard.h"
#include "SceneManager.h"
#include "JoyPad.h"
#include "Sound.h"
#include "score.h"
#include "SoundEffect.h"

// ����΂�
SceneManager *g_pSceneManager;
JoyPad *g_pJoyPad;



HRESULT Init(HWND hWnd, UINT widht, UINT height)
{
	HRESULT hr = S_OK;
	// �V�X�e���ʂ̏����� false�ɂ��Ă������ƁI�I�I�I�I�I�I�I�I�I�I�I�I�I�I
	hr = InitDirextX(hWnd, widht, height, false);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "DirectX�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	hr = InitSound();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "���̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	//�`�`�`�`�`�`�`�`�`�`�`�`
	InitSoundEffect();
	InitBGM();
	//�`�`�`�`�`�`�`�`�`�`�`�`

	//----- �Q�[�����̃I�u�W�F�N�g�̏�����
	// �|���S���̏�����
	hr = InitPolygon(widht, height);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Polygon�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// �L�[�{�[�h�̏�����
	hr = InitKeyboard();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Keyboard�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	
	// �W���C�p�b�h������
	g_pJoyPad = new JoyPad;
	hr = g_pJoyPad->Init();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�Q�[���p�b�h�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// �V�[���̏�����
	g_pSceneManager = new SceneManager;
	// ���߂̃V�[������
	g_pSceneManager->SetStartScene();
	hr = g_pSceneManager->Init(hWnd);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "Scene�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}


	

	return hr;
}
void Uninit()
{
	//�`�`�`�`�`�`�`�`�`�`�`�`
	UninitSoundEffect();
	UninitBGM();
	//�`�`�`�`�`�`�`�`�`�`�`�`
	g_pSceneManager->Uninit();
	delete g_pSceneManager;

	// �W���C�p�b�h�̏I��
	delete g_pJoyPad;
	UninitKeyboard();
	UninitPolygon();
	UninitSound();
	UninitDirectX();
}
void Update()
{
	UpdateKeyboard();
	// �W���C�p�b�h�̍X�V
	g_pJoyPad->Update();

	g_pSceneManager->Update();
}
void Draw()
{
	// ����Đ����łȂ���΃|���S���`��
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
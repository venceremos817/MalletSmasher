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

	// �t�F�[�h�̏�����
	hr = m_Fade.Init();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "fade�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	this->m_nStopCnt = 0;

	switch (m_CurScene)
	{
	case Scene_Title:
		// �o�b�N�O���E���h�̏�����
		hr = InitStage(TutorialStage);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Bg�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// �}���b�g�̏�����
		hr = InitMallet();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Mallet�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// �p�b�N�̏�����
		hr = InitPuck();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Puck�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// �^�C�g���̏�����
		hr=InitTitle();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�^�C�g���̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// �p�[�e�B�N���G�t�F�N�g�̏�����
		m_pEmitMngr = new EmitterMngr;
		m_pEmitMngr->Init(50);
	
		break;
	case Scene_Game:
		// �o�b�N�O���E���h�̏�����
		hr = InitStage(GetSelectStage());
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Bg�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		hr = InitScore();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Score�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// �}���b�g�̏�����
		hr = InitMallet();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Mallet�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// �p�b�N�̏�����
		hr = InitPuck();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Puck�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}
		// �S�[���̏�����
		InitGoal();

		hr = m_pose.Init();
		if (FAILED(hr))
		{
			MessageBox(hWnd, "Pose�̏������Ɏ��s���܂���", "Error", MB_OK | MB_ICONERROR);
			return hr;
		}

		// �G�t�F�N�g�̏�����
		InitEffect();
		// �p�[�e�B�N���G�t�F�N�g�̏�����
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
		// �p�[�e�B�N���G�t�F�N�g�̏I��
		m_pEmitMngr->UnInit();
		delete m_pEmitMngr;
		UninitTitle();
		// �p�b�N�̏I��
		UninitPuck();
		// �}���b�g�̏I��
		UninitMallet();
		// �o�b�N�O���E���h�̏I��
		UninitStage();
		break;
	case Scene_Game:
		// �p�[�e�B�N���G�t�F�N�g�̏I��
		m_pEmitMngr->UnInit();
		delete m_pEmitMngr;
		// �G�t�F�N�g�̏I��
		UninitEffect();
		// �|�[�Y�̏I��
		m_pose.Uninit();
		// �S�[���̏I��
		UninitGoal();
		// �p�b�N�̏I��
		UninitPuck();
		// �}���b�g�̏I��
		UninitMallet();
		// �X�R�A�̏I��
		UninitScore();
		// �o�b�N�O���E���h�̏I��
		UninitStage();

		break;
	default:
		break;
	}
}
void SceneManager::Update()
{
	// ���ł��g����悤�ɂ��Ă���
	// �t�F�[�h
	m_Fade.Update();

	// �V�[���؂�ւ��ŕK�v�Ȃ��̂�����������
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
		// �^�C�g���̍X�V
		UpdateTitle();
		// �}���b�g�̍X�V
		UpdateMallet();
		// �p�b�N�̍X�V
		UpdatePuck();
		// �X�e�[�W�̍X�V
		UpdateStage();
		// �p�[�e�B�N���G�t�F�N�g�̍X�V
		m_pEmitMngr->Update();

		break;
	case Scene_Game:
		m_pose.Update();

		if (!m_pose.IsActive())		// �|�[�Y
		{
			if (m_nStopCnt < 1)			// �q�b�g�X�g�b�v
			{
				// �}���b�g�̍X�V
				UpdateMallet();
				// �p�b�N�̍X�V
				UpdatePuck();
				// �X�e�[�W�̍X�V
				UpdateStage();
				// �X�R�A�̍X�V
				UpdateScore();
				// �G�t�F�N�g�̍X�V
				UpdateEffect();
				// �S�[���̍X�V
				UpdateGoal();
			}
			else
			{
				m_nStopCnt--;
			}
			// �p�[�e�B�N���G�t�F�N�g�̍X�V
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
// ���̃V�[���ŕK�v�Ȃ��̂�`�悷��
void SceneManager::Draw()
{

	switch (m_CurScene)
	{
	case Scene_Title:
		// �o�b�N�O���E���h�̕`��
		DrawStage();
		// �}���b�g�̕`��
		DrawMallet();
		// �p�b�N�̕`��
		DrawPuck();
		// �^�C�g���̕`��
		DrawTitle();
		// �p�[�e�B�N���G�t�F�N�g�̕`��
		m_pEmitMngr->Draw();
		break;
	case Scene_Game:
		// �o�b�N�O���E���h�̕`��
		DrawStage();
		// �X�R�A�̕`��
		DrawScore();
		// �S�[���̕`��
		DrawGoal();
		// �G�t�F�N�g�̕`��
		DrawEffect();
		// �}���b�g�̕`��
		DrawMallet();
		// �p�b�N�̕`��
		DrawPuck();
		// �p�[�e�B�N���G�t�F�N�g�̕`��
		m_pEmitMngr->Draw();
		// �|�[�Y�̕`��
		m_pose.Draw();

		break;
	default:
		break;
	}
	// �t�F�[�h�̕`��
	m_Fade.Draw();
}
// ��������V�[����ݒ肷��
void SceneManager::SetStartScene()
{
	m_CurScene = Scene_Title;
	m_NextScene = Max_Scene;
}
// game.cp���獡�̃V�[�����󂯎��
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


// ����Đ������ǂ���
void SceneManager::IsPlayingVideo(bool bPlaying)
{
	m_isPlayingVideo = bPlaying;
}


bool SceneManager::IsPlayingVideo()
{
	return m_isPlayingVideo;
}

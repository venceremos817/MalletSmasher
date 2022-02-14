#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

// インクルード部
#include<Windows.h>
#include "fade.h"
#include "EmitterMngr.h"
#include "ScenePose.h"


// 列挙体
enum SCENE_KIND
{
	Scene_Title,
	Scene_Game,

	Max_Scene
};
// クラス
class SceneManager
{

private:
	SCENE_KIND m_CurScene;
	SCENE_KIND m_NextScene;
	HWND m_hWnd;
	Fade m_Fade;
	ScenePose m_pose;
	EmitterMngr *m_pEmitMngr;
	int m_nStopCnt;
	bool m_isPlayingVideo;

public:
	HRESULT Init(HWND hWnd);
	void Uninit();
	void Update();
	void Draw();
	void SetStartScene();
	void SetNextScene(SCENE_KIND NextScene);
	Fade GetFade();
	EmitterMngr *GetEmitMngr();
	void SetStopCnt(int);
	void IsPlayingVideo(bool);
	bool IsPlayingVideo();
};


#endif // !__SCENEMANAGER_H__

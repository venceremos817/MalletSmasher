#ifndef __SCENE_POSE_H__
#define __SCENE_POSE_H__

#include "Texture.h"

enum POSE_SELECT
{
	POSE_CONTINUE,
	POSE_EXIT,

	POSE_MAX
};

class ScenePose
{
public:
	HRESULT	Init();
	void	Uninit();
	void	Update();
	void	Draw();

	bool	IsActive();
	bool	IsTitleBack();

private:
	ID3D11ShaderResourceView * m_pTex[POSE_MAX];
	POSE_SELECT		m_SelectCursor;
	bool	m_bOpen;
	bool	m_bTitleBack;
	float	m_fMagnification;
};

#endif // !__SCENE_POSE_H__

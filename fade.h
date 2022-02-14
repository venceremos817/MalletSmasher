#ifndef __FADE_H__
#define __FADE_H__
#include "DirectX.h"

class Fade
{
public:
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Start(float RequireTime, bool Out);
	bool IsFinsh();
	bool IsExecute();

private:

	// �����o
	float m_FadeRequireTime; // ���v����
	float m_FadeElapseTime;  // �o�ߎ���
	bool m_IsOut;            // �t�F�[�h�A�E�g���ǂ���
	
};

#endif // !__FADE_H__

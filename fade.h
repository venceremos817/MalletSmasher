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

	// メンバ
	float m_FadeRequireTime; // 所要時間
	float m_FadeElapseTime;  // 経過時間
	bool m_IsOut;            // フェードアウトかどうか
	
};

#endif // !__FADE_H__

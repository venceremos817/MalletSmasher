#ifndef  __CONVEYOR_H__
#define  __CONVEYOR_H__

#include "Texture.h"
#include "CMyPolygon.h"


class Conveyor
{

private:
	// 変数など
	float m_PosX;
	float m_PosY;
	float m_width;
	float m_height;
	float m_radian;
	float m_anime;
	float m_RotSpd;
	bool m_HitConveyorToPack; // バンパーとパックが当たっているかどうか
	int m_Delay;
	float m_Alpha;
	ID3D11ShaderResourceView *m_pTex;
	CMyPolygon Ploygon;

public:
	// 関数など
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	
	void SetPos(float PosX, float PosY);
	void SetRotSpd(float RotSpd);
};


#endif // ! __CONVEYOR_H__


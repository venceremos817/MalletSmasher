//==============================================
//	CMyPolygon.h
//----------------------------------------------
//	ポリゴンを扱いやすくする
//----------------------------------------------
//	<<<<< 更新履歴 >>>>>
//	20/07/06	製作開始
//				授業で作ったCMyPolygon.hをクラス化
//
//=============================================

#ifndef __C_MY_CMyPolygon_H__
#define __C_MY_CMyPolygon_H__

//===== インクルード =====
#include "Polygon.h"
#include "CMyColor.h"
#include "CommonClass.h"

class CMyPolygon
{
public:
	//--- コンストラクタ ---
	CMyPolygon();
	CMyPolygon(float x, float y, float width, float height);

	// ポリゴンの大きさを設定
	void SetSize(float width, float height);
	void SetSize(Float2);
	// ポリゴンの位置を設定
	void SetPos(float x, float y);
	void SetPos(Float2);
	// ポリゴンの色設定
	void SetColor(float r, float g, float b, float a);
	void SetColor(CMyColor);
	void SetColor(CMyColor, float);
	// アルファ値設定
	void SetAlpha(float);
	// ポリゴンの角度を設定
	void SetAngle(float angle);
	// ポリゴンのUVを設定
	// splitU 横方向の分割数
	// splitV 縦方向の分割数
	void SetSplitUV(int index, int splitU, int splitV);

	// ポリゴンの各頂点の色設定
	void SetVtxColor(int vtx, float r, float g, float b, float a);
	void SetVtxColor(int vtx, CMyColor);
	void SetVtxColor(int vtx, CMyColor,float);

	// ポリゴン描画
	void Draw();

private:
	float	m_fPosX;
	float	m_fPosY;
	float	m_fWidth;
	float	m_fHeight;
	float	m_fRed[4];
	float	m_fGreen[4];
	float	m_fBlue[4];
	float	m_fAlpha[4];
	float	m_fRadian;
	//int		m_nIndex;
	//int		m_nSplitU;
	//int		m_nSplitV;
};


////===== プロトタイプ宣言 =====
//
//// ポリゴンの大きさを設定
//void SetPolygonSize(float width, float height);
//// ポリゴンの位置を設定
//void SetPolygonPos(float x, float y);
//// ポリゴンの角度を設定
//void SetPolygonAngle(float angle);
//// ポリゴンのUVを設定
//// splitU 横方向の分割数
//// splitV 縦方向の分割数
//void SetPolygonSplitUV(int index, int splitU, int splitV);

// 他にも
// 一括で色を設定する
// 一括で透明度を設定する
// UVをいい感じにする

#endif // !__C_MY_CMyPolygon_H__

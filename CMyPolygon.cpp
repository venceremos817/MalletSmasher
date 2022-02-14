//==============================================
//
//	自主勉
//		C++クラス練習 マリオ作る
//----------------------------------------------
//	CMyPolygon.cpp
//----------------------------------------------
//	<<<<< 更新履歴 >>>>>
//	20/07/06	製作開始
//				授業で作ったCMyPolygon.cppをクラス化
//
//=============================================

//===== インクルード部 =====
#include "CMyPolygon.h"
#include <math.h>


//===== 定数定義 =====
#define PI		(3.14159265359f)


//--- コンストラクタ ---
CMyPolygon::CMyPolygon()
{
	m_fPosX = 0;
	m_fPosY = 0;
	m_fWidth = 0;
	m_fHeight = 0;
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = 1.0f;
		m_fGreen[i] = 1.0f;
		m_fBlue[i] = 1.0f;
		m_fAlpha[i] = 1.0f;
	}
	m_fRadian = 0.0f;
	//SetPolygonTexture(NULL);
	//m_nIndex = 0;
	//m_nSplitU = 1;
	//m_nSplitV = 1;
}


//--- コンストラクタ ---
CMyPolygon::CMyPolygon(float x, float y, float width, float height)
{
	m_fPosX = x;
	m_fPosY = y;
	m_fWidth = width;
	m_fHeight = height;
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = 1.0f;
		m_fGreen[i] = 1.0f;
		m_fBlue[i] = 1.0f;
		m_fAlpha[i] = 1.0f;
	}
	m_fRadian = 0.0f;
	//m_nIndex = 0;
	//m_nSplitU = 1;
	//m_nSplitV = 1;
}


//// 頂点の情報を設定
//void CMyPolygon::UpdatePolygon()
//{
//	// 今まではX座標、Y座標を直接指定して表現していた(絶対座標)
//	// 絶対座標だと、少しでも開店するとX座標・Y座標がどの位置に来るかわからなくなる
//
//	// 回転を基準とした座標の表現として極座標を使う
//	// 極座標は角度と距離で座標を表現する
//
//	//----- 極座標で表現 -----
//	// 中心からの距離にそれぞれ計算しなおす
//	float halfW = fWidth / 2.0f;
//	float halfH = fHeight / 2.0f;
//
//	// 中心から四隅への距離を求める
//	// 斜辺の長さ = ( x^2 + y^2 ) ^ 0.5
//	float length = sqrtf(halfW * halfW + halfH * halfH);
//
//	// 四隅に向かう角度を求める
//	// atan2 ... 縦と横の長さを引数に渡すと、その長さで作れる直角三角形の角度を返す(弧度法)
//	float vtxRadian = atan2f(-halfH, halfW);
//	float radian[4] = {
//		atan2f(-halfH,-halfW),		// 左上への角度
//		atan2f(-halfH,halfW),		// 右上への角度
//		atan2f(halfH,-halfW),		// 左下への角度
//		atan2f(halfH,halfW)			// 右下への角度
//	};
//
//	// 極座標から絶対座標へ変換
//	// cosとsinを組み合わせると円周上の座標を表せる
//	//float x = cosf(vtxRadian + g_radian) * length*0.5f;
//	//float y = sinf(vtxRadian + g_radian) * length*0.5f;
//	for (int i = 0; i < 4; i++)
//	{
//		// 各頂点への移動量を極座標から変換して求める
//		float x = cosf(radian[i] + fRadian)*length;
//		float y = sinf(radian[i] + fRadian)*length;
//		// 中心からの距離として頂点座標を設定
//		SetPolygonVtxPos(i,  + x, fPosY + y);
//	}
//
//	//SetPolygonVtxPos(0, g_posX + x, g_posY - y);
//	//SetPolygonVtxPos(1, g_posX + g_width + x, g_posY + y);
//	//SetPolygonVtxPos(2, g_posX - x, g_posY + g_height + y);
//	//SetPolygonVtxPos(3, g_posX + g_width - x, g_posY + g_height - y);
//}





void CMyPolygon::SetSize(float width, float height)
{
	m_fWidth = width;
	m_fHeight = height;
}


void CMyPolygon::SetSize(Float2 size)
{
	m_fWidth = size.x;
	m_fHeight = size.y;
}


void CMyPolygon::SetPos(float x, float y)
{
	m_fPosX = x;
	m_fPosY = y;
}


void CMyPolygon::SetPos(Float2 pos)
{
	m_fPosX = pos.x;
	m_fPosY = pos.y;
}


void CMyPolygon::SetColor(float r, float g, float b, float a)
{
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = r;
		m_fGreen[i] = g;
		m_fBlue[i] = b;
		m_fAlpha[i] = a;
	}
}


void CMyPolygon::SetColor(CMyColor color)
{
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = color.GetRed();
		m_fGreen[i] = color.GetGreen();
		m_fBlue[i] = color.GetBlue();
		m_fAlpha[i] = color.GetAlpha();
	}
}


void CMyPolygon::SetColor(CMyColor color, float alpha)
{
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = color.GetRed();
		m_fGreen[i] = color.GetGreen();
		m_fBlue[i] = color.GetBlue();
		m_fAlpha[i] = alpha;
	}
}


void CMyPolygon::SetAlpha(float alpha)
{
	for (int i = 0; i < 4; i++)
	{
		m_fAlpha[i] = alpha;
	}
}


void CMyPolygon::SetAngle(float angle)
{
	// 45度、90度など分かりやすい角度（度数法）ではなく
	// ラジアン（弧度法）で角度を表現する

	// θ = θ° * π / 180°

	m_fRadian = angle * PI / 180.0f;		// 角度をラジアンに変換

}



void CMyPolygon::SetSplitUV(int index, int splitU, int splitV)
{
	//// 縦と横に区切った空間へ順番に数字が入っているとしたとき
	//// 計算から横の位置と縦の位置を求めることができる

	float UVLeft = (index % splitU) / (float)splitU;
	float UVRight = UVLeft + 1.0f / (float)splitU;
	float UVTop = (index / splitU) / (float)splitV;
	float UVBottom = UVTop + 1.0f / (float)splitV;

	// UVの設定
	SetPolygonVtxUV(0, UVLeft, UVTop);
	SetPolygonVtxUV(1, UVRight, UVTop);
	SetPolygonVtxUV(2, UVLeft, UVBottom);
	SetPolygonVtxUV(3, UVRight, UVBottom);
}


void CMyPolygon::SetVtxColor(int vtx, float r, float g, float b, float a)
{
	m_fRed[vtx] = r;
	m_fGreen[vtx] = g;
	m_fBlue[vtx] = b;
	m_fAlpha[vtx] = a;
}


void CMyPolygon::SetVtxColor(int vtx, CMyColor color)
{
	m_fRed[vtx] = color.GetRed();
	m_fGreen[vtx] = color.GetGreen();
	m_fBlue[vtx] = color.GetBlue();
	m_fAlpha[vtx] = color.GetAlpha();
}


void CMyPolygon::SetVtxColor(int vtx, CMyColor color, float alpha)
{
	m_fRed[vtx] = color.GetRed();
	m_fGreen[vtx] = color.GetGreen();
	m_fBlue[vtx] = color.GetBlue();
	m_fAlpha[vtx] = alpha;
}


void CMyPolygon::Draw()
{
	//----- 極座標で表現 -----
	// 中心からの距離にそれぞれ計算しなおす
	float halfW = m_fWidth * 0.5f;
	float halfH = m_fHeight * 0.5f;

	// 中心から四隅への距離を求める
	// 斜辺の長さ = ( x^2 + y^2 ) ^ 0.5
	float length = sqrtf(halfW * halfW + halfH * halfH);

	// 四隅に向かう角度を求める
	// atan2 ... 縦と横の長さを引数に渡すと、その長さで作れる直角三角形の角度を返す(弧度法)
	float vtxRadian = atan2f(-halfH, halfW);
	float radian[4] = {
		atan2f(-halfH,-halfW),		// 左上への角度
		atan2f(-halfH,halfW),		// 右上への角度
		atan2f(halfH,-halfW),		// 左下への角度
		atan2f(halfH,halfW)			// 右下への角度
	};

	// 極座標から絶対座標へ変換
	for (int i = 0; i < 4; i++)
	{
		// 各頂点への移動量を極座標から変換して求める
		float x = cosf(radian[i] + m_fRadian)*length;
		float y = sinf(radian[i] + m_fRadian)*length;
		// 中心からの距離として頂点座標を設定
		SetPolygonVtxPos(i, m_fPosX + x, m_fPosY + y);
		// 色設定
		SetPolygonVtxColor(i, m_fRed[i], m_fGreen[i], m_fBlue[i], m_fAlpha[i]);
	}

	DrawPolygon();
}

//===== インクルード部 =====
#include "myPolygon.h"
#include <math.h>


//===== 定数定義 =====
#define PI		(3.14159265359f)



//===== グローバル変数 =====
float g_width;
float g_height;
float g_posX;
float g_posY;
float g_radian;


// 頂点の情報を設定
void UpdatePolygon()
{
	// 今まではX座標、Y座標を直接指定して表現していた(絶対座標)
	// 絶対座標だと、少しでも開店するとX座標・Y座標がどの位置に来るかわからなくなる

	// 回転を基準とした座標の表現として極座標を使う
	// 極座標は角度と距離で座標を表現する

	//----- 極座標で表現 -----
	// 中心からの距離にそれぞれ計算しなおす
	float halfW = g_width / 2.0f;
	float halfH = g_height / 2.0f;

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
	// cosとsinを組み合わせると円周上の座標を表せる
	//float x = cosf(vtxRadian + g_radian) * length*0.5f;
	//float y = sinf(vtxRadian + g_radian) * length*0.5f;
	for (int i = 0; i < 4; i++)
	{
		// 各頂点への移動量を極座標から変換して求める
		float x = cosf(radian[i] + g_radian)*length;
		float y = sinf(radian[i] + g_radian)*length;
		// 中心からの距離として頂点座標を設定
		SetPolygonVtxPos(i, g_posX + x, g_posY + y);
	}

	//SetPolygonVtxPos(0, g_posX + x, g_posY - y);
	//SetPolygonVtxPos(1, g_posX + g_width + x, g_posY + y);
	//SetPolygonVtxPos(2, g_posX - x, g_posY + g_height + y);
	//SetPolygonVtxPos(3, g_posX + g_width - x, g_posY + g_height - y);
}





void SetPolygonSize(float width, float height)
{
	g_width = width;
	g_height = height;
	UpdatePolygon();
}





void SetPolygonPos(float x, float y)
{
	g_posX = x;
	g_posY = y;
	UpdatePolygon();
}





void SetPolygonAngle(float angle)
{
	// 45度、90度など分かりやすい角度（度数法）ではなく
	// ラジアン（弧度法）で角度を表現する

	// θ = θ° * π / 180°

	g_radian = angle * PI / 180.0f;		// 角度をラジアンに変換

	UpdatePolygon();
}





void SetPolygonSplitUV(int index, int splitU, int splitV)
{
	//// 縦と横に区切った空間へ順番に数字が入っているとしたとき
	//// 計算から横の位置と縦の位置を求めることができる

	//// 横の分割数で割った余りが横の位置
	//int u = index % splitU;
	//// 縦の分割数で割った結果が縦の位置(小数点以下切り捨て）
	//int v = index / splitV;
	//// 区切った空間の縦幅
	//float w = 1.0 / splitU;
	//// 区切った空間の縦幅
	//float h = 1.0 / splitV;

	float UVLeft = (index % splitU) / (float)splitU;
	float UVRight = UVLeft + 1.0f / (float)splitU;
	float UVTop = (index / splitU) / (float)splitV;
	float UVBottom = UVTop + 1.0f / (float)splitV;

	// UVの設定
	//SetPolygonVtxUV(0, u * w, v * h);
	//SetPolygonVtxUV(1, (u + 1) * w, v * h);
	//SetPolygonVtxUV(2, u * w, (v + 1) * h);
	//SetPolygonVtxUV(3, (u + 1) * w, (v + 1) * h);
	SetPolygonVtxUV(0, UVLeft, UVTop);
	SetPolygonVtxUV(1, UVRight, UVTop);
	SetPolygonVtxUV(2, UVLeft, UVBottom);
	SetPolygonVtxUV(3, UVRight, UVBottom);


	//SetPolygonVtxUV(0, index%splitU * 1.0 / splitU					, index / splitV * 1.0 / splitV);
	//SetPolygonVtxUV(1, index%splitU * 1.0 / splitU + 1.0 / splitU	, index / splitV * 1.0 / splitV);
	//SetPolygonVtxUV(2, index%splitU * 1.0 / splitU					, index / splitV * 1.0 / splitV + 1.0 / splitV);
	//SetPolygonVtxUV(3, index%splitU * 1.0 / splitU + 1.0 / splitU, index / splitV * 1.0 / splitV + 1.0 / splitV);
}
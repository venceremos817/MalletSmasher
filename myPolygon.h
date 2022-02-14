#ifndef __MY_POLYGON_H__
#define __MY_POLYGON_H__

//===== インクルード =====
#include "Polygon.h"



//===== プロトタイプ宣言 =====

// ポリゴンの大きさを設定
void SetPolygonSize(float width, float height);
// ポリゴンの位置を設定
void SetPolygonPos(float x, float y);
// ポリゴンの角度を設定
void SetPolygonAngle(float angle);
// ポリゴンのUVを設定
// splitU 横方向の分割数
// splitV 縦方向の分割数
void SetPolygonSplitUV(int index, int splitU, int splitV);

// 他にも
// 一括で色を設定する
// 一括で透明度を設定する
// UVをいい感じにする

#endif // !__MY_POLYGON_H__

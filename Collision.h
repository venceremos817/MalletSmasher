#ifndef __COLLISION_H__
#define __COLLISION_H__


//===== 構造体宣言 =====
// 当たり判定の矩形
typedef struct
{
	float	posX;
	float	posY;		// 中心座標
	float	width;
	float	height;	// 大きさ
}myRECT;

// 当たり判定の円
typedef struct
{
	float posX, posY;		// 中心座標
	float radius;			// 半径
}myCIRCLE;


//===== クラス =====
class Collision
{
private:
	// 矩形同士の当たり判定
	bool CheckCollisionRectToRect(float PosX1, float PosY1, float PosX2, float PosY2, float width1, float height1, float width2, float height2);
	bool CheckCollisionRectToRect(myRECT rect1, myRECT rect2);
	// 円同士の当たり判定
	bool CheckCollisionCircleToCircle(float PosX1, float PosY1, float PosX2, float PosY2, float radius1, float radius2);
	bool CheckCollisionCircleToCircle(myCIRCLE c1, myCIRCLE c2);
	// 点と矩形の当たり判定
	bool CheckCollisionPointToRect(float PointX, float PointY, float RectPosX, float RectPosY, float width, float height);
	bool CheckCollisionPointToRect(myRECT rect, float posX, float posY);
};


//===== プロトタイプ宣言 =====
// 矩形同士の当たり判定
bool CheckCollisionRectToRect(float PosX1, float PosY1, float PosX2, float PosY2, float width1, float height1, float width2, float height2);

// 円同士の当たり判定
bool CheckCollisionCircleToCircle(float PosX1, float PosY1, float PosX2, float PosY2, float radiud1, float radiud2);

// 点と矩形の当たり判定
bool CheckCollisionPointToRect(float PointX, float PointY, float RectPosX, float RectPosY, float width, float height);

// 天と円の当たり判定
bool CheckColisionPointToCircle(float PointX, float PointY, float CirclePosX, float CirclePosY, float radius);



#endif // !__COLLISION_H__

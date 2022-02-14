//===== インクルード部 =====
#include "Collision.h"
#include <math.h>




//===== クラス関数の実装 =====
bool Collision::CheckCollisionRectToRect(float PosX1, float PosY1, float PosX2, float PosY2, float width1, float height1, float width2, float height2)
{
	float halfWidth1 = width1 * 0.5f;
	float halfHeight1 = height1 * 0.5f;
	float halfWidth2 = width2 * 0.5f;
	float halfHeight2 = height2 * 0.5f;

	if ((PosX2 - halfWidth2 < PosX1 + halfWidth1) &&		// 2の左端 < 1の右端
		(PosX1 - halfWidth1 < PosX2 + halfWidth2))			// 1の左端 < 2の右端
	{
		if ((PosY2 - halfHeight2 < PosY1 + halfHeight1) &&		// 2の上端 < 1の下端
			(PosY1 - halfHeight1 < PosY2 + halfHeight2))		// 1の上端 < 2の下端
		{
			return true;
		}
	}



	return false;
}


bool Collision::CheckCollisionRectToRect(myRECT rect1, myRECT rect2)
{
	float halfWidth1 = rect1.width*0.5f;
	float halfHeight1 = rect1.height*0.5f;
	float halfWidth2 = rect2.width*0.5f;
	float halfHeight2 = rect2.height*0.5f;

	if ((rect2.posX - halfWidth2 < rect1.posX + halfWidth1) &&		// 2の左端 < 1の右端
		(rect1.posX - halfWidth1 < rect2.posX + halfWidth2))			// 1の左端 < 2の右端
	{
		if ((rect2.posY - halfHeight2 < rect1.posY + halfHeight1) &&		// 2の上端 < 1の下端
			(rect1.posY - halfHeight1 < rect2.posY + halfHeight2))		// 1の上端 < 2の下端
		{
			return true;
		}
	}



	return false;
}


bool Collision::CheckCollisionCircleToCircle(float PosX1, float PosY1, float PosX2, float PosY2, float radius1, float radius2)
{
	if (powf((PosX1 - PosX2), 2.0f) + powf((PosY1 - PosY2), 2.0f)		// (円1の中心座標X - 円2の中心座標X)の2乗 + (円1の中心座標Y - 円2の中心座標Y)の2乗
		<= powf((radius1 + radius2), 2.0f))						// (円1の半径+円2の半径)の2乗
	{
		return true;
	}

	return false;
}


bool Collision::CheckCollisionCircleToCircle(myCIRCLE c1, myCIRCLE c2)
{
	if (powf((c1.posX - c2.posX), 2.0f) + powf((c1.posY - c2.posY), 2.0f)		// (円1の中心座標X - 円2の中心座標X)の2乗 + (円1の中心座標Y - 円2の中心座標Y)の2乗
		<= powf((c1.radius + c2.radius), 2.0f))								// (円1の半径+円2の半径)の2乗
	{
		return true;
	}

	return false;
}


bool Collision::CheckCollisionPointToRect(float PointX, float PointY, float RectPosX, float RectPosY, float width, float height)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	if ((RectPosX - halfWidth <= PointX) && (PointX <= RectPosX + halfWidth))
	{
		if ((RectPosY - halfHeight <= PointY) && (PointY <= RectPosY + halfHeight))
			return true;
	}

	return false;
}


bool Collision::CheckCollisionPointToRect(myRECT rect, float posX, float posY)
{
	float halfWidth = rect.width * 0.5f;
	float halfHeight = rect.height * 0.5f;

	if ((rect.posX - halfWidth <= posX) && (posX <= rect.posX + halfWidth))
	{
		if ((rect.posY - halfHeight <= posY) && (posY <= rect.posY + halfHeight))
			return true;
	}

	return false;
}







// 矩形同士の当たり判定
bool CheckCollisionRectToRect(float PosX1, float PosY1, float PosX2, float PosY2, float width1, float height1, float width2, float height2)
{
	float halfWidth1 = width1 * 0.5f;
	float halfHeight1 = height1 * 0.5f;
	float halfWidth2 = width2 * 0.5f;
	float halfHeight2 = height2 * 0.5f;

	if ((PosX2 - halfWidth2 < PosX1 + halfWidth1) &&		// 2の左端 < 1の右端
		(PosX1 - halfWidth1 < PosX2 + halfWidth2))			// 1の左端 < 2の右端
	{
		if ((PosY2 - halfHeight2 < PosY1 + halfHeight1) &&		// 2の上端 < 1の下端
			(PosY1 - halfHeight1 < PosY2 + halfHeight2))		// 1の上端 < 2の下端
		{
			return true;
		}
	}



	return false;
}





// 円同士の当たり判定
bool CheckCollisionCircleToCircle(float PosX1, float PosY1, float PosX2, float PosY2, float radius1, float radius2)
{
	if (powf((PosX1 - PosX2), 2.0f) + powf((PosY1 - PosY2), 2.0f)		// (円1の中心座標X - 円2の中心座標X)の2乗 + (円1の中心座標Y - 円2の中心座標Y)の2乗
		<= powf((radius1 + radius2), 2.0f))						// (円1の半径+円2の半径)の2乗
	{
		return true;
	}

	return false;
}





// 点と矩形の当たり判定
bool CheckCollisionPointToRect(float PointX, float PointY, float RectPosX, float RectPosY, float width, float height)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	if ((RectPosX - halfWidth <= PointX) && (PointX <= RectPosX + halfWidth))
	{
		if ((RectPosY - halfHeight <= PointY) && (PointY <= RectPosY + halfHeight))
			return true;
	}

	return false;
}




// 点と円の当たり判定
bool CheckColisionPointToCircle(float PointX, float PointY, float CirclePosX, float CirclePosY, float radius)
{
	float distance;		// 点と円の距離^2

	distance = (PointX - CirclePosX)*(PointX - CirclePosX) + (PointY - CirclePosY)*(PointY - CirclePosY);
	if (distance <= radius)
		return true;

	return false;
}
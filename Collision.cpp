//===== �C���N���[�h�� =====
#include "Collision.h"
#include <math.h>




//===== �N���X�֐��̎��� =====
bool Collision::CheckCollisionRectToRect(float PosX1, float PosY1, float PosX2, float PosY2, float width1, float height1, float width2, float height2)
{
	float halfWidth1 = width1 * 0.5f;
	float halfHeight1 = height1 * 0.5f;
	float halfWidth2 = width2 * 0.5f;
	float halfHeight2 = height2 * 0.5f;

	if ((PosX2 - halfWidth2 < PosX1 + halfWidth1) &&		// 2�̍��[ < 1�̉E�[
		(PosX1 - halfWidth1 < PosX2 + halfWidth2))			// 1�̍��[ < 2�̉E�[
	{
		if ((PosY2 - halfHeight2 < PosY1 + halfHeight1) &&		// 2�̏�[ < 1�̉��[
			(PosY1 - halfHeight1 < PosY2 + halfHeight2))		// 1�̏�[ < 2�̉��[
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

	if ((rect2.posX - halfWidth2 < rect1.posX + halfWidth1) &&		// 2�̍��[ < 1�̉E�[
		(rect1.posX - halfWidth1 < rect2.posX + halfWidth2))			// 1�̍��[ < 2�̉E�[
	{
		if ((rect2.posY - halfHeight2 < rect1.posY + halfHeight1) &&		// 2�̏�[ < 1�̉��[
			(rect1.posY - halfHeight1 < rect2.posY + halfHeight2))		// 1�̏�[ < 2�̉��[
		{
			return true;
		}
	}



	return false;
}


bool Collision::CheckCollisionCircleToCircle(float PosX1, float PosY1, float PosX2, float PosY2, float radius1, float radius2)
{
	if (powf((PosX1 - PosX2), 2.0f) + powf((PosY1 - PosY2), 2.0f)		// (�~1�̒��S���WX - �~2�̒��S���WX)��2�� + (�~1�̒��S���WY - �~2�̒��S���WY)��2��
		<= powf((radius1 + radius2), 2.0f))						// (�~1�̔��a+�~2�̔��a)��2��
	{
		return true;
	}

	return false;
}


bool Collision::CheckCollisionCircleToCircle(myCIRCLE c1, myCIRCLE c2)
{
	if (powf((c1.posX - c2.posX), 2.0f) + powf((c1.posY - c2.posY), 2.0f)		// (�~1�̒��S���WX - �~2�̒��S���WX)��2�� + (�~1�̒��S���WY - �~2�̒��S���WY)��2��
		<= powf((c1.radius + c2.radius), 2.0f))								// (�~1�̔��a+�~2�̔��a)��2��
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







// ��`���m�̓����蔻��
bool CheckCollisionRectToRect(float PosX1, float PosY1, float PosX2, float PosY2, float width1, float height1, float width2, float height2)
{
	float halfWidth1 = width1 * 0.5f;
	float halfHeight1 = height1 * 0.5f;
	float halfWidth2 = width2 * 0.5f;
	float halfHeight2 = height2 * 0.5f;

	if ((PosX2 - halfWidth2 < PosX1 + halfWidth1) &&		// 2�̍��[ < 1�̉E�[
		(PosX1 - halfWidth1 < PosX2 + halfWidth2))			// 1�̍��[ < 2�̉E�[
	{
		if ((PosY2 - halfHeight2 < PosY1 + halfHeight1) &&		// 2�̏�[ < 1�̉��[
			(PosY1 - halfHeight1 < PosY2 + halfHeight2))		// 1�̏�[ < 2�̉��[
		{
			return true;
		}
	}



	return false;
}





// �~���m�̓����蔻��
bool CheckCollisionCircleToCircle(float PosX1, float PosY1, float PosX2, float PosY2, float radius1, float radius2)
{
	if (powf((PosX1 - PosX2), 2.0f) + powf((PosY1 - PosY2), 2.0f)		// (�~1�̒��S���WX - �~2�̒��S���WX)��2�� + (�~1�̒��S���WY - �~2�̒��S���WY)��2��
		<= powf((radius1 + radius2), 2.0f))						// (�~1�̔��a+�~2�̔��a)��2��
	{
		return true;
	}

	return false;
}





// �_�Ƌ�`�̓����蔻��
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




// �_�Ɖ~�̓����蔻��
bool CheckColisionPointToCircle(float PointX, float PointY, float CirclePosX, float CirclePosY, float radius)
{
	float distance;		// �_�Ɖ~�̋���^2

	distance = (PointX - CirclePosX)*(PointX - CirclePosX) + (PointY - CirclePosY)*(PointY - CirclePosY);
	if (distance <= radius)
		return true;

	return false;
}
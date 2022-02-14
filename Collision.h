#ifndef __COLLISION_H__
#define __COLLISION_H__


//===== �\���̐錾 =====
// �����蔻��̋�`
typedef struct
{
	float	posX;
	float	posY;		// ���S���W
	float	width;
	float	height;	// �傫��
}myRECT;

// �����蔻��̉~
typedef struct
{
	float posX, posY;		// ���S���W
	float radius;			// ���a
}myCIRCLE;


//===== �N���X =====
class Collision
{
private:
	// ��`���m�̓����蔻��
	bool CheckCollisionRectToRect(float PosX1, float PosY1, float PosX2, float PosY2, float width1, float height1, float width2, float height2);
	bool CheckCollisionRectToRect(myRECT rect1, myRECT rect2);
	// �~���m�̓����蔻��
	bool CheckCollisionCircleToCircle(float PosX1, float PosY1, float PosX2, float PosY2, float radius1, float radius2);
	bool CheckCollisionCircleToCircle(myCIRCLE c1, myCIRCLE c2);
	// �_�Ƌ�`�̓����蔻��
	bool CheckCollisionPointToRect(float PointX, float PointY, float RectPosX, float RectPosY, float width, float height);
	bool CheckCollisionPointToRect(myRECT rect, float posX, float posY);
};


//===== �v���g�^�C�v�錾 =====
// ��`���m�̓����蔻��
bool CheckCollisionRectToRect(float PosX1, float PosY1, float PosX2, float PosY2, float width1, float height1, float width2, float height2);

// �~���m�̓����蔻��
bool CheckCollisionCircleToCircle(float PosX1, float PosY1, float PosX2, float PosY2, float radiud1, float radiud2);

// �_�Ƌ�`�̓����蔻��
bool CheckCollisionPointToRect(float PointX, float PointY, float RectPosX, float RectPosY, float width, float height);

// �V�Ɖ~�̓����蔻��
bool CheckColisionPointToCircle(float PointX, float PointY, float CirclePosX, float CirclePosY, float radius);



#endif // !__COLLISION_H__

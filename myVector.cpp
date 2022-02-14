#include "myVector.h"
#include <math.h>

// �R���X�g���N�^
myVector2::myVector2()
{
	x = 0;
	y = 0;
}

myVector2::myVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

// �v�f�̐ݒ�
void myVector2::operator()(float x, float y)
{
	this->x = x;
	this->y = y;
}

// �x�N�g�� + �x�N�g��
myVector2 myVector2::operator+(myVector2 vec)
{
	myVector2 ret;

	ret.x = x + vec.x;
	ret.y = x + vec.y;

	return ret;
}

// �x�N�g�� - �x�N�g��
myVector2 myVector2::operator-(myVector2 vec)
{
	myVector2 ret;

	ret.x = x - vec.x;
	ret.y = y - vec.y;

	return ret;
}

// �x�N�g�� * �X�J���[
myVector2 myVector2::operator*(float sc)
{
	myVector2 ret;

	ret.x = x * sc;
	ret.y = x * sc;

	return ret;
}


// �X�J���[ * �x�N�g��
myVector2 operator*(float scalar, myVector2 vec)
{
	myVector2 ret;

	ret.x = vec.x * scalar;
	ret.y = vec.y * scalar;

	return ret;
}


// �x�N�g�� / �X�J���[
myVector2 myVector2::operator/(float scalar)
{
	myVector2 ret;

	ret.x = x / scalar;
	ret.y = y / scalar;

	return ret;
}


// ����
float myVector2::dot(myVector2 vec1, myVector2 vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}


// �O��
float myVector2::cross(myVector2 vec1, myVector2 vec2)
{
	return vec1.x*vec2.y - vec1.y*vec2.x;
}


// 2�̃x�N�g���̂Ȃ��p 0...��
float myVector2::GetRadian(myVector2 vec1, myVector2 vec2)
{
	// �[���x�N�g���Ȃ�Ȃ��p���o���Ȃ�
	if (GetLength(vec1) == 0 || GetLength(vec2) == 0)
		return 0;
	// rad = arccos( A dot B / |A|*|B| )
	return acosf(dot(vec1, vec2) / (GetLength(vec1)*GetLength(vec2)));
}


// �x�N�g���̑傫���擾
float myVector2::GetLength()
{
	return sqrtf(x * x + y * y);
}


float myVector2::GetLength(myVector2 vec)
{
	return sqrtf(vec.x*vec.x + vec.y*vec.y);
}


// �P�ʃx�N�g�����擾
myVector2 myVector2::GetNormalize()
{
	myVector2 ret;
	
	ret.x = x;
	ret.y = y;

	return ret / GetLength();
}
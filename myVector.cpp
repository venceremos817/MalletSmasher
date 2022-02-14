#include "myVector.h"
#include <math.h>

// コンストラクタ
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

// 要素の設定
void myVector2::operator()(float x, float y)
{
	this->x = x;
	this->y = y;
}

// ベクトル + ベクトル
myVector2 myVector2::operator+(myVector2 vec)
{
	myVector2 ret;

	ret.x = x + vec.x;
	ret.y = x + vec.y;

	return ret;
}

// ベクトル - ベクトル
myVector2 myVector2::operator-(myVector2 vec)
{
	myVector2 ret;

	ret.x = x - vec.x;
	ret.y = y - vec.y;

	return ret;
}

// ベクトル * スカラー
myVector2 myVector2::operator*(float sc)
{
	myVector2 ret;

	ret.x = x * sc;
	ret.y = x * sc;

	return ret;
}


// スカラー * ベクトル
myVector2 operator*(float scalar, myVector2 vec)
{
	myVector2 ret;

	ret.x = vec.x * scalar;
	ret.y = vec.y * scalar;

	return ret;
}


// ベクトル / スカラー
myVector2 myVector2::operator/(float scalar)
{
	myVector2 ret;

	ret.x = x / scalar;
	ret.y = y / scalar;

	return ret;
}


// 内積
float myVector2::dot(myVector2 vec1, myVector2 vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}


// 外積
float myVector2::cross(myVector2 vec1, myVector2 vec2)
{
	return vec1.x*vec2.y - vec1.y*vec2.x;
}


// 2つのベクトルのなす角 0...π
float myVector2::GetRadian(myVector2 vec1, myVector2 vec2)
{
	// ゼロベクトルならなす角を出せない
	if (GetLength(vec1) == 0 || GetLength(vec2) == 0)
		return 0;
	// rad = arccos( A dot B / |A|*|B| )
	return acosf(dot(vec1, vec2) / (GetLength(vec1)*GetLength(vec2)));
}


// ベクトルの大きさ取得
float myVector2::GetLength()
{
	return sqrtf(x * x + y * y);
}


float myVector2::GetLength(myVector2 vec)
{
	return sqrtf(vec.x*vec.x + vec.y*vec.y);
}


// 単位ベクトルを取得
myVector2 myVector2::GetNormalize()
{
	myVector2 ret;
	
	ret.x = x;
	ret.y = y;

	return ret / GetLength();
}
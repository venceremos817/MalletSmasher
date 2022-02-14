#include "CommonClass.h"


Float2::Float2(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Float2::operator()(float x, float y)
{
	this->x = x;
	this->y = y;
}

Float2 Float2::operator+(Float2 b)
{
	Float2 ret;
	ret.x = this->x + b.x;
	ret.y = this->y + b.y;

	return ret;
}

Float2 Float2::operator-(Float2 b)
{
	Float2 ret;
	ret.x = this->x - b.x;
	ret.y = this->y - b.y;

	return ret;
}




Float3::Float3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


void Float3::operator()(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


Float3 Float3::operator+(Float3 b)
{
	Float3 ret;
	ret.x = this->x + b.x;
	ret.y = this->y + b.y;
	ret.z = this->z + b.z;

	return ret;
}


Float3 Float3::operator-(Float3 b)
{
	Float3 ret;
	ret.x = this->x - b.x;
	ret.y = this->y - b.y;
	ret.z = this->z - b.z;

	return ret;
}





Float4::Float4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


void Float4::operator()(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


Float4 Float4::operator+(Float4 b)
{
	Float4 ret;
	ret.x = this->x + b.x;
	ret.y = this->y + b.y;
	ret.z = this->z + b.z;
	ret.w = this->w + b.w;

	return ret;
}


Float4 Float4::operator-(Float4 b)
{
	Float4 ret;
	ret.x = this->x - b.x;
	ret.y = this->y - b.y;
	ret.z = this->z - b.z;
	ret.w = this->w - b.w;

	return ret;
}
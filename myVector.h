#pragma once

//===== 2次元ベクトル =====
class myVector2
{
private:

public:
	float x, y;

	myVector2();

	myVector2(float x, float y);

	// 要素のセット
	void operator()(float x, float y);

	//===== ベクトルの演算 =====
	// ベクトル + ベクトル
	myVector2 operator+(myVector2 vec);

	// ベクトル - ベクトル
	myVector2 operator-(myVector2 vec);

	// ベクトル * スカラー
	myVector2 operator*(float scalar);

	// ベクトル / スカラー
	myVector2 operator/(float scalar);

	// ベクトルの大きさ取得
	float GetLength();

	static float GetLength(myVector2 vec);

	// 内積
	static float dot(myVector2 vec1, myVector2 vec2);

	// 外積
	static float cross(myVector2 vec1, myVector2 vec2);

	// 2つのベクトルのなす角 0...π
	static float GetRadian(myVector2 vec1, myVector2 vec2);

	// 単位ベクトルを取得
	myVector2 GetNormalize();

};

// スカラー * ベクトル
myVector2 operator*(float scalar, myVector2 vec);
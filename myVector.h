#pragma once

//===== 2�����x�N�g�� =====
class myVector2
{
private:

public:
	float x, y;

	myVector2();

	myVector2(float x, float y);

	// �v�f�̃Z�b�g
	void operator()(float x, float y);

	//===== �x�N�g���̉��Z =====
	// �x�N�g�� + �x�N�g��
	myVector2 operator+(myVector2 vec);

	// �x�N�g�� - �x�N�g��
	myVector2 operator-(myVector2 vec);

	// �x�N�g�� * �X�J���[
	myVector2 operator*(float scalar);

	// �x�N�g�� / �X�J���[
	myVector2 operator/(float scalar);

	// �x�N�g���̑傫���擾
	float GetLength();

	static float GetLength(myVector2 vec);

	// ����
	static float dot(myVector2 vec1, myVector2 vec2);

	// �O��
	static float cross(myVector2 vec1, myVector2 vec2);

	// 2�̃x�N�g���̂Ȃ��p 0...��
	static float GetRadian(myVector2 vec1, myVector2 vec2);

	// �P�ʃx�N�g�����擾
	myVector2 GetNormalize();

};

// �X�J���[ * �x�N�g��
myVector2 operator*(float scalar, myVector2 vec);
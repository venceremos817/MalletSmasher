#ifndef __MY_POLYGON_H__
#define __MY_POLYGON_H__

//===== �C���N���[�h =====
#include "Polygon.h"



//===== �v���g�^�C�v�錾 =====

// �|���S���̑傫����ݒ�
void SetPolygonSize(float width, float height);
// �|���S���̈ʒu��ݒ�
void SetPolygonPos(float x, float y);
// �|���S���̊p�x��ݒ�
void SetPolygonAngle(float angle);
// �|���S����UV��ݒ�
// splitU �������̕�����
// splitV �c�����̕�����
void SetPolygonSplitUV(int index, int splitU, int splitV);

// ���ɂ�
// �ꊇ�ŐF��ݒ肷��
// �ꊇ�œ����x��ݒ肷��
// UV�����������ɂ���

#endif // !__MY_POLYGON_H__

//===== �C���N���[�h�� =====
#include "myPolygon.h"
#include <math.h>


//===== �萔��` =====
#define PI		(3.14159265359f)



//===== �O���[�o���ϐ� =====
float g_width;
float g_height;
float g_posX;
float g_posY;
float g_radian;


// ���_�̏���ݒ�
void UpdatePolygon()
{
	// ���܂ł�X���W�AY���W�𒼐ڎw�肵�ĕ\�����Ă���(��΍��W)
	// ��΍��W���ƁA�����ł��J�X�����X���W�EY���W���ǂ̈ʒu�ɗ��邩�킩��Ȃ��Ȃ�

	// ��]����Ƃ������W�̕\���Ƃ��ċɍ��W���g��
	// �ɍ��W�͊p�x�Ƌ����ō��W��\������

	//----- �ɍ��W�ŕ\�� -----
	// ���S����̋����ɂ��ꂼ��v�Z���Ȃ���
	float halfW = g_width / 2.0f;
	float halfH = g_height / 2.0f;

	// ���S����l���ւ̋��������߂�
	// �Εӂ̒��� = ( x^2 + y^2 ) ^ 0.5
	float length = sqrtf(halfW * halfW + halfH * halfH);

	// �l���Ɍ������p�x�����߂�
	// atan2 ... �c�Ɖ��̒����������ɓn���ƁA���̒����ō��钼�p�O�p�`�̊p�x��Ԃ�(�ʓx�@)
	float vtxRadian = atan2f(-halfH, halfW);
	float radian[4] = {
		atan2f(-halfH,-halfW),		// ����ւ̊p�x
		atan2f(-halfH,halfW),		// �E��ւ̊p�x
		atan2f(halfH,-halfW),		// �����ւ̊p�x
		atan2f(halfH,halfW)			// �E���ւ̊p�x
	};

	// �ɍ��W�����΍��W�֕ϊ�
	// cos��sin��g�ݍ��킹��Ɖ~����̍��W��\����
	//float x = cosf(vtxRadian + g_radian) * length*0.5f;
	//float y = sinf(vtxRadian + g_radian) * length*0.5f;
	for (int i = 0; i < 4; i++)
	{
		// �e���_�ւ̈ړ��ʂ��ɍ��W����ϊ����ċ��߂�
		float x = cosf(radian[i] + g_radian)*length;
		float y = sinf(radian[i] + g_radian)*length;
		// ���S����̋����Ƃ��Ē��_���W��ݒ�
		SetPolygonVtxPos(i, g_posX + x, g_posY + y);
	}

	//SetPolygonVtxPos(0, g_posX + x, g_posY - y);
	//SetPolygonVtxPos(1, g_posX + g_width + x, g_posY + y);
	//SetPolygonVtxPos(2, g_posX - x, g_posY + g_height + y);
	//SetPolygonVtxPos(3, g_posX + g_width - x, g_posY + g_height - y);
}





void SetPolygonSize(float width, float height)
{
	g_width = width;
	g_height = height;
	UpdatePolygon();
}





void SetPolygonPos(float x, float y)
{
	g_posX = x;
	g_posY = y;
	UpdatePolygon();
}





void SetPolygonAngle(float angle)
{
	// 45�x�A90�x�ȂǕ�����₷���p�x�i�x���@�j�ł͂Ȃ�
	// ���W�A���i�ʓx�@�j�Ŋp�x��\������

	// �� = �Ɓ� * �� / 180��

	g_radian = angle * PI / 180.0f;		// �p�x�����W�A���ɕϊ�

	UpdatePolygon();
}





void SetPolygonSplitUV(int index, int splitU, int splitV)
{
	//// �c�Ɖ��ɋ�؂�����Ԃ֏��Ԃɐ����������Ă���Ƃ����Ƃ�
	//// �v�Z���牡�̈ʒu�Əc�̈ʒu�����߂邱�Ƃ��ł���

	//// ���̕������Ŋ������]�肪���̈ʒu
	//int u = index % splitU;
	//// �c�̕������Ŋ��������ʂ��c�̈ʒu(�����_�ȉ��؂�̂āj
	//int v = index / splitV;
	//// ��؂�����Ԃ̏c��
	//float w = 1.0 / splitU;
	//// ��؂�����Ԃ̏c��
	//float h = 1.0 / splitV;

	float UVLeft = (index % splitU) / (float)splitU;
	float UVRight = UVLeft + 1.0f / (float)splitU;
	float UVTop = (index / splitU) / (float)splitV;
	float UVBottom = UVTop + 1.0f / (float)splitV;

	// UV�̐ݒ�
	//SetPolygonVtxUV(0, u * w, v * h);
	//SetPolygonVtxUV(1, (u + 1) * w, v * h);
	//SetPolygonVtxUV(2, u * w, (v + 1) * h);
	//SetPolygonVtxUV(3, (u + 1) * w, (v + 1) * h);
	SetPolygonVtxUV(0, UVLeft, UVTop);
	SetPolygonVtxUV(1, UVRight, UVTop);
	SetPolygonVtxUV(2, UVLeft, UVBottom);
	SetPolygonVtxUV(3, UVRight, UVBottom);


	//SetPolygonVtxUV(0, index%splitU * 1.0 / splitU					, index / splitV * 1.0 / splitV);
	//SetPolygonVtxUV(1, index%splitU * 1.0 / splitU + 1.0 / splitU	, index / splitV * 1.0 / splitV);
	//SetPolygonVtxUV(2, index%splitU * 1.0 / splitU					, index / splitV * 1.0 / splitV + 1.0 / splitV);
	//SetPolygonVtxUV(3, index%splitU * 1.0 / splitU + 1.0 / splitU, index / splitV * 1.0 / splitV + 1.0 / splitV);
}
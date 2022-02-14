//==============================================
//
//	�����
//		C++�N���X���K �}���I���
//----------------------------------------------
//	CMyPolygon.cpp
//----------------------------------------------
//	<<<<< �X�V���� >>>>>
//	20/07/06	����J�n
//				���Ƃō����CMyPolygon.cpp���N���X��
//
//=============================================

//===== �C���N���[�h�� =====
#include "CMyPolygon.h"
#include <math.h>


//===== �萔��` =====
#define PI		(3.14159265359f)


//--- �R���X�g���N�^ ---
CMyPolygon::CMyPolygon()
{
	m_fPosX = 0;
	m_fPosY = 0;
	m_fWidth = 0;
	m_fHeight = 0;
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = 1.0f;
		m_fGreen[i] = 1.0f;
		m_fBlue[i] = 1.0f;
		m_fAlpha[i] = 1.0f;
	}
	m_fRadian = 0.0f;
	//SetPolygonTexture(NULL);
	//m_nIndex = 0;
	//m_nSplitU = 1;
	//m_nSplitV = 1;
}


//--- �R���X�g���N�^ ---
CMyPolygon::CMyPolygon(float x, float y, float width, float height)
{
	m_fPosX = x;
	m_fPosY = y;
	m_fWidth = width;
	m_fHeight = height;
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = 1.0f;
		m_fGreen[i] = 1.0f;
		m_fBlue[i] = 1.0f;
		m_fAlpha[i] = 1.0f;
	}
	m_fRadian = 0.0f;
	//m_nIndex = 0;
	//m_nSplitU = 1;
	//m_nSplitV = 1;
}


//// ���_�̏���ݒ�
//void CMyPolygon::UpdatePolygon()
//{
//	// ���܂ł�X���W�AY���W�𒼐ڎw�肵�ĕ\�����Ă���(��΍��W)
//	// ��΍��W���ƁA�����ł��J�X�����X���W�EY���W���ǂ̈ʒu�ɗ��邩�킩��Ȃ��Ȃ�
//
//	// ��]����Ƃ������W�̕\���Ƃ��ċɍ��W���g��
//	// �ɍ��W�͊p�x�Ƌ����ō��W��\������
//
//	//----- �ɍ��W�ŕ\�� -----
//	// ���S����̋����ɂ��ꂼ��v�Z���Ȃ���
//	float halfW = fWidth / 2.0f;
//	float halfH = fHeight / 2.0f;
//
//	// ���S����l���ւ̋��������߂�
//	// �Εӂ̒��� = ( x^2 + y^2 ) ^ 0.5
//	float length = sqrtf(halfW * halfW + halfH * halfH);
//
//	// �l���Ɍ������p�x�����߂�
//	// atan2 ... �c�Ɖ��̒����������ɓn���ƁA���̒����ō��钼�p�O�p�`�̊p�x��Ԃ�(�ʓx�@)
//	float vtxRadian = atan2f(-halfH, halfW);
//	float radian[4] = {
//		atan2f(-halfH,-halfW),		// ����ւ̊p�x
//		atan2f(-halfH,halfW),		// �E��ւ̊p�x
//		atan2f(halfH,-halfW),		// �����ւ̊p�x
//		atan2f(halfH,halfW)			// �E���ւ̊p�x
//	};
//
//	// �ɍ��W�����΍��W�֕ϊ�
//	// cos��sin��g�ݍ��킹��Ɖ~����̍��W��\����
//	//float x = cosf(vtxRadian + g_radian) * length*0.5f;
//	//float y = sinf(vtxRadian + g_radian) * length*0.5f;
//	for (int i = 0; i < 4; i++)
//	{
//		// �e���_�ւ̈ړ��ʂ��ɍ��W����ϊ����ċ��߂�
//		float x = cosf(radian[i] + fRadian)*length;
//		float y = sinf(radian[i] + fRadian)*length;
//		// ���S����̋����Ƃ��Ē��_���W��ݒ�
//		SetPolygonVtxPos(i,  + x, fPosY + y);
//	}
//
//	//SetPolygonVtxPos(0, g_posX + x, g_posY - y);
//	//SetPolygonVtxPos(1, g_posX + g_width + x, g_posY + y);
//	//SetPolygonVtxPos(2, g_posX - x, g_posY + g_height + y);
//	//SetPolygonVtxPos(3, g_posX + g_width - x, g_posY + g_height - y);
//}





void CMyPolygon::SetSize(float width, float height)
{
	m_fWidth = width;
	m_fHeight = height;
}


void CMyPolygon::SetSize(Float2 size)
{
	m_fWidth = size.x;
	m_fHeight = size.y;
}


void CMyPolygon::SetPos(float x, float y)
{
	m_fPosX = x;
	m_fPosY = y;
}


void CMyPolygon::SetPos(Float2 pos)
{
	m_fPosX = pos.x;
	m_fPosY = pos.y;
}


void CMyPolygon::SetColor(float r, float g, float b, float a)
{
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = r;
		m_fGreen[i] = g;
		m_fBlue[i] = b;
		m_fAlpha[i] = a;
	}
}


void CMyPolygon::SetColor(CMyColor color)
{
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = color.GetRed();
		m_fGreen[i] = color.GetGreen();
		m_fBlue[i] = color.GetBlue();
		m_fAlpha[i] = color.GetAlpha();
	}
}


void CMyPolygon::SetColor(CMyColor color, float alpha)
{
	for (int i = 0; i < 4; i++)
	{
		m_fRed[i] = color.GetRed();
		m_fGreen[i] = color.GetGreen();
		m_fBlue[i] = color.GetBlue();
		m_fAlpha[i] = alpha;
	}
}


void CMyPolygon::SetAlpha(float alpha)
{
	for (int i = 0; i < 4; i++)
	{
		m_fAlpha[i] = alpha;
	}
}


void CMyPolygon::SetAngle(float angle)
{
	// 45�x�A90�x�ȂǕ�����₷���p�x�i�x���@�j�ł͂Ȃ�
	// ���W�A���i�ʓx�@�j�Ŋp�x��\������

	// �� = �Ɓ� * �� / 180��

	m_fRadian = angle * PI / 180.0f;		// �p�x�����W�A���ɕϊ�

}



void CMyPolygon::SetSplitUV(int index, int splitU, int splitV)
{
	//// �c�Ɖ��ɋ�؂�����Ԃ֏��Ԃɐ����������Ă���Ƃ����Ƃ�
	//// �v�Z���牡�̈ʒu�Əc�̈ʒu�����߂邱�Ƃ��ł���

	float UVLeft = (index % splitU) / (float)splitU;
	float UVRight = UVLeft + 1.0f / (float)splitU;
	float UVTop = (index / splitU) / (float)splitV;
	float UVBottom = UVTop + 1.0f / (float)splitV;

	// UV�̐ݒ�
	SetPolygonVtxUV(0, UVLeft, UVTop);
	SetPolygonVtxUV(1, UVRight, UVTop);
	SetPolygonVtxUV(2, UVLeft, UVBottom);
	SetPolygonVtxUV(3, UVRight, UVBottom);
}


void CMyPolygon::SetVtxColor(int vtx, float r, float g, float b, float a)
{
	m_fRed[vtx] = r;
	m_fGreen[vtx] = g;
	m_fBlue[vtx] = b;
	m_fAlpha[vtx] = a;
}


void CMyPolygon::SetVtxColor(int vtx, CMyColor color)
{
	m_fRed[vtx] = color.GetRed();
	m_fGreen[vtx] = color.GetGreen();
	m_fBlue[vtx] = color.GetBlue();
	m_fAlpha[vtx] = color.GetAlpha();
}


void CMyPolygon::SetVtxColor(int vtx, CMyColor color, float alpha)
{
	m_fRed[vtx] = color.GetRed();
	m_fGreen[vtx] = color.GetGreen();
	m_fBlue[vtx] = color.GetBlue();
	m_fAlpha[vtx] = alpha;
}


void CMyPolygon::Draw()
{
	//----- �ɍ��W�ŕ\�� -----
	// ���S����̋����ɂ��ꂼ��v�Z���Ȃ���
	float halfW = m_fWidth * 0.5f;
	float halfH = m_fHeight * 0.5f;

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
	for (int i = 0; i < 4; i++)
	{
		// �e���_�ւ̈ړ��ʂ��ɍ��W����ϊ����ċ��߂�
		float x = cosf(radian[i] + m_fRadian)*length;
		float y = sinf(radian[i] + m_fRadian)*length;
		// ���S����̋����Ƃ��Ē��_���W��ݒ�
		SetPolygonVtxPos(i, m_fPosX + x, m_fPosY + y);
		// �F�ݒ�
		SetPolygonVtxColor(i, m_fRed[i], m_fGreen[i], m_fBlue[i], m_fAlpha[i]);
	}

	DrawPolygon();
}

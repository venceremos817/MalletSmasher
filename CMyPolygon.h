//==============================================
//	CMyPolygon.h
//----------------------------------------------
//	�|���S���������₷������
//----------------------------------------------
//	<<<<< �X�V���� >>>>>
//	20/07/06	����J�n
//				���Ƃō����CMyPolygon.h���N���X��
//
//=============================================

#ifndef __C_MY_CMyPolygon_H__
#define __C_MY_CMyPolygon_H__

//===== �C���N���[�h =====
#include "Polygon.h"
#include "CMyColor.h"
#include "CommonClass.h"

class CMyPolygon
{
public:
	//--- �R���X�g���N�^ ---
	CMyPolygon();
	CMyPolygon(float x, float y, float width, float height);

	// �|���S���̑傫����ݒ�
	void SetSize(float width, float height);
	void SetSize(Float2);
	// �|���S���̈ʒu��ݒ�
	void SetPos(float x, float y);
	void SetPos(Float2);
	// �|���S���̐F�ݒ�
	void SetColor(float r, float g, float b, float a);
	void SetColor(CMyColor);
	void SetColor(CMyColor, float);
	// �A���t�@�l�ݒ�
	void SetAlpha(float);
	// �|���S���̊p�x��ݒ�
	void SetAngle(float angle);
	// �|���S����UV��ݒ�
	// splitU �������̕�����
	// splitV �c�����̕�����
	void SetSplitUV(int index, int splitU, int splitV);

	// �|���S���̊e���_�̐F�ݒ�
	void SetVtxColor(int vtx, float r, float g, float b, float a);
	void SetVtxColor(int vtx, CMyColor);
	void SetVtxColor(int vtx, CMyColor,float);

	// �|���S���`��
	void Draw();

private:
	float	m_fPosX;
	float	m_fPosY;
	float	m_fWidth;
	float	m_fHeight;
	float	m_fRed[4];
	float	m_fGreen[4];
	float	m_fBlue[4];
	float	m_fAlpha[4];
	float	m_fRadian;
	//int		m_nIndex;
	//int		m_nSplitU;
	//int		m_nSplitV;
};


////===== �v���g�^�C�v�錾 =====
//
//// �|���S���̑傫����ݒ�
//void SetPolygonSize(float width, float height);
//// �|���S���̈ʒu��ݒ�
//void SetPolygonPos(float x, float y);
//// �|���S���̊p�x��ݒ�
//void SetPolygonAngle(float angle);
//// �|���S����UV��ݒ�
//// splitU �������̕�����
//// splitV �c�����̕�����
//void SetPolygonSplitUV(int index, int splitU, int splitV);

// ���ɂ�
// �ꊇ�ŐF��ݒ肷��
// �ꊇ�œ����x��ݒ肷��
// UV�����������ɂ���

#endif // !__C_MY_CMyPolygon_H__

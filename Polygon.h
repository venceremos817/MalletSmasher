#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "DirectX.h"

/**
 * @brief �|���S��������
 * @param[in] width ��ʉ���
 * @param[in] height ��ʏc��
 * @return ��������
 */
HRESULT InitPolygon(UINT width, UINT height);

/**
 * @brief �|���S���I������
 */
void UninitPolygon();

/**
 * @brief �|���S���`�揈��
 */
void DrawPolygon();


/**
 * @brief ���_���W�ݒ�
 * @param[in] idx �ݒ肷�钸�_
 * @param[in] x X���W
 * @param[in] y Y���W
 */
void SetPolygonVtxPos(unsigned char idx, float x, float y);

/**
 * @brief ���_�J���[�ݒ�
 * @param[in] idx �ݒ肷�钸�_
 * @param[in] r �ԐF
 * @param[in] g �ΐF
 * @param[in] b �F
 * @param[in] a ���ߐF
 */
void SetPolygonVtxColor(unsigned char idx, float r, float g, float b, float a);

/**
 * @brief ���_UV�ݒ�
 * @param[in] idx �ݒ肷�钸�_
 * @param[in] u U���W
 * @param[in] v V���W
 */
void SetPolygonVtxUV(unsigned char idx, float u, float v);

/**
 * @brief �e�N�X�`���\��t��
 * @param[in] pTexture �\��t����e�N�X�`��
 */
void SetPolygonTexture(ID3D11ShaderResourceView *pTexture);

#endif // __POLYGON_H__
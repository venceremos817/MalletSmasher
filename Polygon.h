#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "DirectX.h"

/**
 * @brief ポリゴン初期化
 * @param[in] width 画面横幅
 * @param[in] height 画面縦幅
 * @return 処理結果
 */
HRESULT InitPolygon(UINT width, UINT height);

/**
 * @brief ポリゴン終了処理
 */
void UninitPolygon();

/**
 * @brief ポリゴン描画処理
 */
void DrawPolygon();


/**
 * @brief 頂点座標設定
 * @param[in] idx 設定する頂点
 * @param[in] x X座標
 * @param[in] y Y座標
 */
void SetPolygonVtxPos(unsigned char idx, float x, float y);

/**
 * @brief 頂点カラー設定
 * @param[in] idx 設定する頂点
 * @param[in] r 赤色
 * @param[in] g 緑色
 * @param[in] b 青色
 * @param[in] a 透過色
 */
void SetPolygonVtxColor(unsigned char idx, float r, float g, float b, float a);

/**
 * @brief 頂点UV設定
 * @param[in] idx 設定する頂点
 * @param[in] u U座標
 * @param[in] v V座標
 */
void SetPolygonVtxUV(unsigned char idx, float u, float v);

/**
 * @brief テクスチャ貼り付け
 * @param[in] pTexture 貼り付けるテクスチャ
 */
void SetPolygonTexture(ID3D11ShaderResourceView *pTexture);

#endif // __POLYGON_H__
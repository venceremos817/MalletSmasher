#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "DirectX.h"
#include "CommonClass.h"
#include "CMyColor.h"

#define MALLETRADIUS (130)
// 初期位置_1P
#define MALLETDEFAULT_POSX_1P (180)
#define MALLETDEFAULT_POSY_1P (SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4)
// 初期位置_2P
#define MALLETDEFAULT_POSX_2P (SCREEN_WIDTH - 180)
#define MALLETDEFAULT_POSY_2P (SCREEN_HEIGHT /4)
// マレットのスピード
#define MALLET_SPEED (22.0f)

// 列挙体
enum MalletState
{
	MalletActive,
	MalletNonActive
};


// 構造体
typedef struct 
{
	float CurPosx;  // 今の位置X
	float CurPosy;  // 今の位置Y
	float PrePosx;  // 過去の位置X
	float PrePosy;  // 過去の位置Y
	float radius;	// 半径
	float movex;	// 移動量X
	float movey;	// 移動量Y
	Float4 color;
	float Power;	// 力
	bool SmashButton;		// スマッシュ
	int SmashFrame;			// スマッシュの有効フレーム
	int	SmashChargeFrame;	// スマッシュボタン押している時間
	float mass;		// 質量
	MalletState State;
	int HitStopFrame;

}MalletInfo;

HRESULT InitMallet();
void UninitMallet();
void UpdateMallet();
void DrawMallet();

// ゲッター
MalletInfo *GetMallet_1P();
MalletInfo *GetMallet_2P();
ID3D11ShaderResourceView *GetCircleTex();

#endif // !__PLAYER_H__



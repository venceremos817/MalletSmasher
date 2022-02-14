#ifndef __PUCK_H__
#define __PUCK_H__


#include "DirectX.h"
#include "CommonClass.h"
#include "CMyColor.h"

#define PUCKRADIUS (100)
#define MAX_PUCK (10)
#define MAX_PRIMITIVE (30)
#define MAX_PACK_SPEED (50)

// 列挙型　パックの種類
enum PuckKind
{
	NormalPuck,
	DualPuck,
	BonusPuck,
	MiniPuck,
	GhostPuck,

	MaxPuckKind
};



// 構造体
// 残像
typedef struct
{
	Float2 Pos;
	float radius;
	Float4 Color;
	bool bDisp;
}AfterEffectInfo;

typedef struct
{
	float CurPosx; // 今の位置X
	float CurPosy; // 今の位置Y
	float PrePosx; // 過去の位置X
	float PrePosy; // 過去の位置Y
	AfterEffectInfo AE[MAX_PRIMITIVE];
	float radian;  // ラジアン
	float radius;	// 半径
	float movex;    // 速さ
	float movey;    // 速さ
	float mass;		// 質量
	float Alpha;    // α値
	CMyColor color;
	PuckKind Kind;  // パックの種類
	bool exist;		// 有るか無いか
	int Score;		// 何点入るか
	bool HitConveyor; // コンベアと当たるかどうか
	int HitStopFrame;

}PuckInfo;

HRESULT InitPuck();
void UninitPuck();
void UpdatePuck();
void DrawPuck();
void CreatePuck(PuckKind Kind,float Posx,float Posy);
void CreateMaxPuck();

// ゲッター
PuckInfo *GetPuckInfo();



#endif //__PUCK_H__


#ifndef __PUCK_H__
#define __PUCK_H__


#include "DirectX.h"
#include "CommonClass.h"
#include "CMyColor.h"

#define PUCKRADIUS (100)
#define MAX_PUCK (10)
#define MAX_PRIMITIVE (30)
#define MAX_PACK_SPEED (50)

// �񋓌^�@�p�b�N�̎��
enum PuckKind
{
	NormalPuck,
	DualPuck,
	BonusPuck,
	MiniPuck,
	GhostPuck,

	MaxPuckKind
};



// �\����
// �c��
typedef struct
{
	Float2 Pos;
	float radius;
	Float4 Color;
	bool bDisp;
}AfterEffectInfo;

typedef struct
{
	float CurPosx; // ���̈ʒuX
	float CurPosy; // ���̈ʒuY
	float PrePosx; // �ߋ��̈ʒuX
	float PrePosy; // �ߋ��̈ʒuY
	AfterEffectInfo AE[MAX_PRIMITIVE];
	float radian;  // ���W�A��
	float radius;	// ���a
	float movex;    // ����
	float movey;    // ����
	float mass;		// ����
	float Alpha;    // ���l
	CMyColor color;
	PuckKind Kind;  // �p�b�N�̎��
	bool exist;		// �L�邩������
	int Score;		// ���_���邩
	bool HitConveyor; // �R���x�A�Ɠ����邩�ǂ���
	int HitStopFrame;

}PuckInfo;

HRESULT InitPuck();
void UninitPuck();
void UpdatePuck();
void DrawPuck();
void CreatePuck(PuckKind Kind,float Posx,float Posy);
void CreateMaxPuck();

// �Q�b�^�[
PuckInfo *GetPuckInfo();



#endif //__PUCK_H__


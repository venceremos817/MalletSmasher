#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "DirectX.h"
#include "CommonClass.h"
#include "CMyColor.h"

#define MALLETRADIUS (130)
// �����ʒu_1P
#define MALLETDEFAULT_POSX_1P (180)
#define MALLETDEFAULT_POSY_1P (SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4)
// �����ʒu_2P
#define MALLETDEFAULT_POSX_2P (SCREEN_WIDTH - 180)
#define MALLETDEFAULT_POSY_2P (SCREEN_HEIGHT /4)
// �}���b�g�̃X�s�[�h
#define MALLET_SPEED (22.0f)

// �񋓑�
enum MalletState
{
	MalletActive,
	MalletNonActive
};


// �\����
typedef struct 
{
	float CurPosx;  // ���̈ʒuX
	float CurPosy;  // ���̈ʒuY
	float PrePosx;  // �ߋ��̈ʒuX
	float PrePosy;  // �ߋ��̈ʒuY
	float radius;	// ���a
	float movex;	// �ړ���X
	float movey;	// �ړ���Y
	Float4 color;
	float Power;	// ��
	bool SmashButton;		// �X�}�b�V��
	int SmashFrame;			// �X�}�b�V���̗L���t���[��
	int	SmashChargeFrame;	// �X�}�b�V���{�^�������Ă��鎞��
	float mass;		// ����
	MalletState State;
	int HitStopFrame;

}MalletInfo;

HRESULT InitMallet();
void UninitMallet();
void UpdateMallet();
void DrawMallet();

// �Q�b�^�[
MalletInfo *GetMallet_1P();
MalletInfo *GetMallet_2P();
ID3D11ShaderResourceView *GetCircleTex();

#endif // !__PLAYER_H__



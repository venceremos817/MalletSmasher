// �E�B���h�E���b�Z�[�W�ł��L�[���͎͂擾�ł��邪�A
// �\�[�X�t�@�C���ɕ������邱�Ƃł��ꂼ��̋@�\����������Ǝ����ł���
// 

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

//===== �C���N���[�h�� =====
#include <Windows.h>


//===== �v���g�^�C�v�錾 =====
HRESULT InitKeyboard();
void UninitKeyboard();
void UpdateKeyboard();

#ifdef _DEBUG
void DrawKeyboard();
#endif // _DEBUG


// �w�肳�ꂽ�L�[��������Ă��邩���ׂ�֐�
// ������nKey�Œ��ׂ����L�[���w��
bool IsKeyPress(int nKey);

// �w�肳�ꂽ�L�[�����A���߂ĉ����ꂽ�����ׂ�֐�
bool IsKeyTrigger(int nKey);

// �w�肳�ꂽ�L�[����w�����ꂽ�u�Ԃ����ׂ�֐�
bool IsKeyRelease(int nKey);

// ���s�[�g���͂����ׂ���ƕ֗�
// �{�^�������������Ă���ԁA�ŏ��̈��Ƃ��΂炭�o���Ă���̓��͂𒲂ׂ�
bool IsKeyRepeat(int nKey);






#endif // !__KEYBOARD_H__

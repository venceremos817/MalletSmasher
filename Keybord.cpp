//===== �C���N���[�h�� =====
#include "Keyboard.h"


//===== �萔��` =====
#define KEY_WAIT	(20)	// ���s�[�g���͊J�n�܂ł̑҂�����
#define KEY_TICK	(4)		// ���s�[�g���͂̔����Ԋu


//===== �O���[�o���ϐ� =====
BYTE g_key[256];			// �S�ẴL�[(256��j�̓��͏���ێ�����z��
BYTE g_preKey[256];			// �ЂƂO�̃L�[�̓��͏��
BYTE g_keyCount[256];		// ���ׂẴL�[�̓��͌p�����Ԃ�ێ�����


HRESULT InitKeyboard()
{
	// ����K�v�Ȃ�
	return S_OK;
}


void UninitKeyboard()
{
	// ����K�v�Ȃ�
}


void UpdateKeyboard()
{
	// �L�[�{�[�h�̓��͂𒲂ׂ�ɂ�3�p�^�[���̕��@������

	// �p�^�[���P	�E�B���h�E���b�Z�[�W�ŃL�[���͂̃��b�Z�[�W���������Ă��邩�𒲂ׂ�֐����Ăяo��
	// GetKeyState(VK_LEFT);
	// �����ɒ��ׂ����L�[���w��


	// �p�^�[���Q	���ډ�����Ă��邩�ǂ����������ɒ��ׂ�֐�
	// GetAsyncKeyState(VK_LEFT);
	// �Q�[���̓��͂̓t���[�����Ɍ��݂̓��͏���ۑ����Ă����āA�K�v�ɂȂ����ӏ���
	// �ۑ����Ă����������g���Ē��ׂ�
	// �Q�̊֐��͌ʂɒ��ׂ镪�ɂ͗ǂ����A��ʂɓ��͂𒲂ׂ�̂Ɍ����Ȃ�


	// �p�^�[���R	�S�ẴL�[�̓��͏��𒲂ׂ�֐�
	// �����ɃL�[�̓��͏����i�[����z�񂪕K�v
	memcpy(g_preKey, g_key, sizeof(g_key));		// �ЂƂO�̃L�[���͏��ޔ�

	GetKeyboardState(g_key);

	//--- ���s�[�g���͗p���� ---
	for (int i = 0; i < 256; i++)
	{
		// �L�[���͂�����ꍇ�A�A�����ĉ�����Ă���
		// ���Ԃ𑝂₷
		if (IsKeyPress(i))
		{
			g_keyCount[i]++;
		}
		else
		{
			// �L�[���͂��Ȃ���΃L�[�͘A�����ĉ�����Ă��Ȃ�(�o�ߎ��Ԃ�0)
			g_keyCount[i] = 0;
		}
	}

}


#ifdef _DEBUG
void DrawKeyboard()
{
	// �L�[�{�[�h�͂����܂œ��͂���邾���Ȃ̂ŕ`��͕K�v�Ȃ�
	// �������A�J���r���̃f�o�b�O�Ƃ��ăL�[�{�[�h�̓��͏󋵂�\�����邱�Ƃ͂���
}
#endif // _DEBUG




// �w�肳�ꂽ�L�[��������Ă��邩���ׂ�֐�
// ������nKey�Œ��ׂ����L�[���w��
bool IsKeyPress(int nKey)
{
	// �w�肳�ꂽ�L�[�����̂܂ܔz��̓Y�����ɂȂ�
	// �L�[�����͂��ꂽ���ǂ����̏��� 8bit �ځichar�^�̍ŏ�ʃr�b�g�j�Ɋi�[����Ă���
	return g_key[nKey] & 0x80;
}



// �w�肳�ꂽ�L�[�����A���߂ĉ����ꂽ�����ׂ�֐�
bool IsKeyTrigger(int nKey)
{
	// �ЂƂO�̓��͂ƁA���݂̓��͂�0,1�̃r�b�g�ɒu�������Đ�������Ƃ킩��₷��
	// �g���K�[�͂P�O��������Ă��Ȃ�(0)�ŁA���͉�����Ă���(1)���ƃg���K�[���(1)�ɂȂ�
	// (g_preKey[nKey]&0x80)==0 && (g_key[nKey]&0x80)==1

	// ���̎��ł��g���K�[�𒲂ׂ��邪�A�����Ȃ�B�r�b�g���Z���g���ĒZ���L�q
	// �r�b�g���Z�̂ق����������x����

	return (g_preKey[nKey] ^ g_key[nKey]) & g_key[nKey] & 0x80;
}



// �w�肳�ꂽ�L�[����w�����ꂽ�u�Ԃ����ׂ�֐�
bool IsKeyRelease(int nKey)
{
	return (g_preKey[nKey] ^ g_key[nKey])&g_preKey[nKey] & 0x80;
}




bool IsKeyRepeat(int nKey)
{
	// �ŏ��ɉ����ꂽ�Ƃ��͔�������
	if (g_keyCount[nKey] == 1)
	{
		return true;
	}
	// ���΂炭���Ԃ������Ă���A�Ăє�������
	else if (g_keyCount[nKey] > KEY_WAIT)
	{
		// ���Ԋu�œ��͂𔽉�������
		if (g_keyCount[nKey] % KEY_TICK == 0)
			return true;
	}

	return false;
}
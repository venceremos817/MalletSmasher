//===== インクルード部 =====
#include "Keyboard.h"


//===== 定数定義 =====
#define KEY_WAIT	(20)	// リピート入力開始までの待ち時間
#define KEY_TICK	(4)		// リピート入力の発生間隔


//===== グローバル変数 =====
BYTE g_key[256];			// 全てのキー(256種）の入力情報を保持する配列
BYTE g_preKey[256];			// ひとつ前のキーの入力情報
BYTE g_keyCount[256];		// すべてのキーの入力継続時間を保持する


HRESULT InitKeyboard()
{
	// 現状必要なし
	return S_OK;
}


void UninitKeyboard()
{
	// 現状必要なし
}


void UpdateKeyboard()
{
	// キーボードの入力を調べるには3パターンの方法がある

	// パターン１	ウィンドウメッセージでキー入力のメッセージが発生しているかを調べる関数を呼び出す
	// GetKeyState(VK_LEFT);
	// 引数に調べたいキーを指定


	// パターン２	直接押されているかどうかをすぐに調べる関数
	// GetAsyncKeyState(VK_LEFT);
	// ゲームの入力はフレーム毎に現在の入力情報を保存しておいて、必要になった箇所で
	// 保存しておいた情報を使って調べる
	// ２の関数は個別に調べる分には良いが、大量に入力を調べるのに向かない


	// パターン３	全てのキーの入力情報を調べる関数
	// 引数にキーの入力情報を格納する配列が必要
	memcpy(g_preKey, g_key, sizeof(g_key));		// ひとつ前のキー入力情報退避

	GetKeyboardState(g_key);

	//--- リピート入力用処理 ---
	for (int i = 0; i < 256; i++)
	{
		// キー入力がある場合、連続して押されている
		// 時間を増やす
		if (IsKeyPress(i))
		{
			g_keyCount[i]++;
		}
		else
		{
			// キー入力がなければキーは連続して押されていない(経過時間は0)
			g_keyCount[i] = 0;
		}
	}

}


#ifdef _DEBUG
void DrawKeyboard()
{
	// キーボードはあくまで入力を取るだけなので描画は必要ない
	// ただし、開発途中のデバッグとしてキーボードの入力状況を表示することはある
}
#endif // _DEBUG




// 指定されたキーが押されているか調べる関数
// 引数のnKeyで調べたいキーを指定
bool IsKeyPress(int nKey)
{
	// 指定されたキーがそのまま配列の添え字になる
	// キーが入力されたかどうかの情報は 8bit 目（char型の最上位ビット）に格納されている
	return g_key[nKey] & 0x80;
}



// 指定されたキーが今、初めて押されたか調べる関数
bool IsKeyTrigger(int nKey)
{
	// ひとつ前の入力と、現在の入力を0,1のビットに置き換えて整理するとわかりやすい
	// トリガーは１つ前が押されていない(0)で、今は押されている(1)だとトリガー状態(1)になる
	// (g_preKey[nKey]&0x80)==0 && (g_key[nKey]&0x80)==1

	// ↑の式でもトリガーを調べられるが、長くなる。ビット演算を使って短く記述
	// ビット演算のほうが処理速度早い

	return (g_preKey[nKey] ^ g_key[nKey]) & g_key[nKey] & 0x80;
}



// 指定されたキーから指が離れた瞬間か調べる関数
bool IsKeyRelease(int nKey)
{
	return (g_preKey[nKey] ^ g_key[nKey])&g_preKey[nKey] & 0x80;
}




bool IsKeyRepeat(int nKey)
{
	// 最初に押されたときは反応する
	if (g_keyCount[nKey] == 1)
	{
		return true;
	}
	// しばらく時間がたってから、再び反応する
	else if (g_keyCount[nKey] > KEY_WAIT)
	{
		// 一定間隔で入力を反応させる
		if (g_keyCount[nKey] % KEY_TICK == 0)
			return true;
	}

	return false;
}
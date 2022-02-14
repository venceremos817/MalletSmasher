// ウィンドウメッセージでもキー入力は取得できるが、
// ソースファイルに分割することでそれぞれの機能がすっきりと実装できる
// 

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

//===== インクルード部 =====
#include <Windows.h>


//===== プロトタイプ宣言 =====
HRESULT InitKeyboard();
void UninitKeyboard();
void UpdateKeyboard();

#ifdef _DEBUG
void DrawKeyboard();
#endif // _DEBUG


// 指定されたキーが押されているか調べる関数
// 引数のnKeyで調べたいキーを指定
bool IsKeyPress(int nKey);

// 指定されたキーが今、初めて押されたか調べる関数
bool IsKeyTrigger(int nKey);

// 指定されたキーから指が離れた瞬間か調べる関数
bool IsKeyRelease(int nKey);

// リピート入力も調べられると便利
// ボタンを押し続けている間、最初の一回としばらく経ってからの入力を調べる
bool IsKeyRepeat(int nKey);






#endif // !__KEYBOARD_H__

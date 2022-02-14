// インクルードガード
#ifndef __DIRECTX_H__
#define __DIRECTX_H__


// ライブラリの読み込み
// .cppの関数が一つにまとまったファイル
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"winmm.lib")
#include <Windows.h>
#include <d3d11.h>	// DirectX11の機能を使う！！！！！！
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>


// マクロ
#define SAFE_RELEASE(p)		if(p) { p->Release(); p = NULL; }


// ===== プロトタイプ宣言 =====
HRESULT InitDirextX(HWND hWnd, UINT width, UINT height, bool fullScreen);
void UninitDirectX();
void BeginDrawDirectX();
void EndDrawDirectX();

ID3D11Device *GetDevice();
ID3D11DeviceContext *GetContext();

// αブレンド、加算合成切り替え
void SetAddBlend(bool isUse);





#endif // !__DIRECTX_H__


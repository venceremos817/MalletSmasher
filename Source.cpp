//===== インクルード部 =====
#include <Windows.h>
#include "Game.h"
#include "Source.h"
#include <stdio.h>
#include "resource.h"

#pragma comment(lib,"winmm.lib")		// timeGetTime()用

//===== 定数定義 =====
#define _CRT_SECURE_NO_WARNINGS 

//===== プロトタイプ宣言 =====
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
);



// エントリポイント
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	//===== ウィンドウ作成の流れ =====
	/*
	① ウィンドウクラス情報の設定
	-ウィンドウを作る前に、OSに今からこんなウィンドウ作りますよ！
	というのを教えておく

	② ウィンドウの作成
	-登録した情報を元にウィンドウを作成

	③ ウィンドウの表示
	-作成だけでは画面に表示されない
	個別に表示してあげる
	*/

	//----- 変数宣言 -----
	WNDCLASSEX wcex;	// ウィンドウクラス情報を設定する構造体
	HWND hWnd;			// 作成したウィンドウのハンドル
	MSG message;

	// ① ウィンドウクラス情報の設定
	ZeroMemory(&wcex, sizeof(wcex));	// 構造体の各メンバを0初期化
	wcex.hInstance = hInstance;			// これから作るウィンドウとアプリケーションを紐づけ
	wcex.lpszClassName = "Class Name";	// ウィンドウクラス情報を識別する名前
	wcex.lpfnWndProc = WndProc;			// ウィンドウプロシージャの指定
	wcex.style = CS_CLASSDC;			// ウィンドウクラスのスタイル(特徴)を指定
	wcex.cbSize = sizeof(WNDCLASSEX);	// 構造体のサイズを指定
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));	// タスクバーに表示するアイコンを指定
	wcex.hIconSm = wcex.hIcon;			// 16 * 16 サイズのアイコン
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);		// カーソル
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// 背景の色指定

																	// ウィンドウクラス情報の登録
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "ウィンドウクラス情報の登録に失敗しました", "エラー", MB_OK || MB_ICONERROR);
		return 0;
	}

	// ② ウィンドウの作成
	hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,		// 追加でウィンドウのスタイル(特徴)を指定
		wcex.lpszClassName,			// ウィンドウを作成するためのウィンドウクラスを指定
		"マレットスマッシャー",		// キャプション部分に表示する名前
		WS_CAPTION | WS_SYSMENU,	// 基本のウィンドウスタイルを設定
		CW_USEDEFAULT,				// ウィンドウの表示位置 X
		CW_USEDEFAULT,				// ウィンドウの表示位置 Y
		SCREEN_WIDTH,				// ウィンドウの横幅
		SCREEN_HEIGHT,				// ウィンドウの縦幅
		HWND_DESKTOP,				// 親ウィンドウの指定
		NULL,						// メニューの指定
		hInstance,					// ウィンドウを紐づけているアプリケーション
		NULL						// 不要(昔の名残)
	);

	// ③ ウィンドウの表示
	ShowWindow(
		hWnd,			// 表示するウィンドウの指定
		nCmdShow		// 表示方法
	);
	UpdateWindow(hWnd);	// ウィンドウの中身をリフレッシュ

	// 初期化処理
	Init(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

	// 時間計測の精度を指定
	timeBeginPeriod(1);			// 1ミリ秒間隔

	//----- FPS制御の初期化 -----
	int fpsCount = 0;		// 1秒間の処理回数
	DWORD countStartTime = timeGetTime();	// 計測の開始時間
	DWORD preExexTime = countStartTime;		// 前回の実行時間




	//----- ウィンドウの管理 -----
	while (1)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))		// OSで発生したメッセージをこのプログラムのキューに受け取る
		{
			if (!GetMessage(&message, NULL, 0, 0))				// キューに格納されたメッセージを取り出す
			{
				break;
			}
			else
			{
				TranslateMessage(&message);			// 受け取ったメッセージをウィンドウプロシージャに送信
				DispatchMessage(&message);
			}
		}
		else
		{
			// FPSの制御
			DWORD nowTime = timeGetTime();		// 現在時刻
			if (nowTime - countStartTime >= 1000)
			{
#ifdef _DEBUG
				// タイトル部分にFPSを表示
				char fpsText[20];
				sprintf(fpsText, "FPS:%d", fpsCount);
				SetWindowText(hWnd, fpsText);
#endif // _DEBUG
				// 次の1秒間の計測準備
				countStartTime = nowTime;
				fpsCount = 0;
			}
			// ゲームの処理
			// 60FPS(16ミリ秒)経過するまでは処理しない
			if (nowTime - preExexTime >= 1000 / 60)
			{
				Update();
				Draw();

				fpsCount++;		// ゲームの処理が一回行われた
				preExexTime = nowTime;
			}
		}
	}


	//----- 終了処理 -----
	timeEndPeriod(1);		// 時間計測の精度を戻す
	Uninit();
	UnregisterClass(wcex.lpszClassName, hInstance);		// ウィンドウクラス情報の登録解除

	return 0;
}


// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	// 受け取ったメッセージ毎に処理
	switch (message)
	{
	case WM_DESTROY:			// ウィンドウが閉じられたとき
		PostQuitMessage(0);		// WM_QUITメッセージの発行 プログラム完全終了させる命令
		break;


	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;

		default:
			break;
		}

		break;


	default:
		break;
	}


	// ほかのメッセージは基本の処理を行わせる
	return DefWindowProc(
		hWnd, message,
		wParam, lParam
	);
}
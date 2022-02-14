//===== インクルード部 =====
#include "DirectX.h"


using namespace DirectX;


//===== グローバル変数 =====
ID3D11Device			*g_pDevice;			// GPUに渡すためのデータの作成を管理
ID3D11DeviceContext		*g_pContext;		// メモリ上のデータをGPUで描画するための橋渡し
IDXGISwapChain			*g_pSwapChain;		// 複数のバックバッファを管理
ID3D11RenderTargetView	*g_pRenderTarget;	// GPUに渡す描き込み先
ID3D11BlendState		*g_pBlendState[2];	// 画像合成方法の設定データ

/**
 *	@概要	DirectX初期化
 *	@引数	width	ウィンドウ横幅
 *			height	ウィンドウ縦幅
 *			fullScreen	フルスクリーン指定
 *	@戻り値	初期化の成否
 */
HRESULT InitDirextX(HWND hWnd, UINT width, UINT height, bool fullScreen)
{
	HRESULT hr;		// 関数の実行結果

	//----- スワップチェーンの作成 -----
	// Windowの画面に直接描画を行うのではなく、BackBufferと呼ばれる目に見えない領域に書き込みを行い、
	// 一定のタイミングでWindowの画面に表示を行う

	// -- スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC scDesc;
	ZeroMemory(&scDesc, sizeof(scDesc));	// 0で初期化
	// バックバッファの大きさ
	scDesc.BufferDesc.Width = width;
	scDesc.BufferDesc.Height = height;
	// バックバッファで扱える色の範囲を指定
	// R 8bit , G 8bit , B 8bit , α 8bit
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 必ず1指定
	scDesc.SampleDesc.Count = 1;
	// 画面の更新頻度(リフレッシュレート)
	scDesc.BufferDesc.RefreshRate.Numerator = 1000;		// 分母
	scDesc.BufferDesc.RefreshRate.Denominator = 1;		// 分子
	// バックバッファの司代応方法を指定
	// 最終的に画面へ出力する色の掻き込み先として使用
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 作成するバックバッファの数
	scDesc.BufferCount = 1;
	// バックバッファの表示先のウィンドウを指定
	scDesc.OutputWindow = hWnd;
	// 
	if (fullScreen)
	{
		scDesc.Windowed = FALSE;
	}
	else
	{
		scDesc.Windowed = TRUE;
	}

	//----- スワップチェーンの作成(DirectXの初期化)-----
	//-- 作成に必要なパラメータを用意

	// ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	// GPUで描画(基本)
		D3D_DRIVER_TYPE_WARP,		// WARPドライバで描画
		D3D_DRIVER_TYPE_REFERENCE,	// より高精度（低速）
	};
	// ドライバの配列の要素数
	UINT numDriver = sizeof(driverTypes) / sizeof(driverTypes[0]);

	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		// それぞれGPUがどのレベルのDirectXに対応するのかを示す
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	// 機能レベル配列の要素数
	UINT numFeatureLevel = sizeof(featureLevels) / sizeof(featureLevels[0]);

	//-- スワップチェーンの作成(DirectXの初期化)
	D3D_FEATURE_LEVEL useLevel;		//実際に使用した機能レベル
	// ドライバを一つずつ確認
	for (UINT i = 0; i < numDriver; ++i)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,					// ゲームであまり使わない NULLでOK
			driverTypes[i],			// 利用するドライバ
			NULL, 0,				// ゲームであまり使わない
			featureLevels,			// 利用する機能レベルの一覧
			numFeatureLevel,		// 機能レベルの数
			D3D11_SDK_VERSION,		// 特にどのバージョンのDirectXを使用するのか
			&scDesc,				// スワップチェーンの設定
			&g_pSwapChain,			// 作成したスワップチェーンの格納先
			&g_pDevice,				// 作成したDeviceの格納先
			&useLevel,				// 実際に利用した機能レベル
			&g_pContext				// 作成したContextの格納先
		);

		// 作成が成功したら終了
		if (SUCCEEDED(hr))
			break;
	}

	// 作成が一つも成功しなかった
	if (FAILED(hr))
	{
		return E_FAIL;		// 失敗を表すパラメータを返す
	}

	//--- レンダーターゲットの作成
	// バックバッファのポインタを取得
	ID3D11Texture2D *pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	// バックバッファのポインタをもとにレンダーターゲット作成
	hr = g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTarget);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	// 使用するレンダーターゲットを設定
	g_pContext->OMSetRenderTargets(1, &g_pRenderTarget, NULL);


	// --- アルファブレンド(画像の合成方法) の設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	// 色	(最終的に表示される色の計算方法を指定)
	// 表示される色 * 表示される側の透明度
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	// 元の色 * ( 1 - 表示される側の透明度 )
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	// 表示される色 と 元の色 の合成方法 (足し算)
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// アルファ
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	// 対象
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// アルファブレンドのデータを生成
	hr = g_pDevice->CreateBlendState(&blendDesc, &g_pBlendState[0]);
	if (FAILED(hr))
	{
		return hr;
	}
	// 加算合成のデータを作成
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	// 加算合成用のパラメータになっているblendDescで新しく作成
	hr = g_pDevice->CreateBlendState(&blendDesc, &g_pBlendState[1]);
	if (FAILED(hr))
	{
		return hr;
	}

	// 合成方法の設定
	SetAddBlend(false);		// falseならαブレンド

	//--- テクスチャの繰り返し設定
	// サンプラーステート
	D3D11_SAMPLER_DESC smpDesc;
	ID3D11SamplerState *pSampler;
	ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
	// テクスチャの繰り返し方法
	// ほかの設定...CLAMP	端の色を引っ張る(デフォルト時の指定)
	//				WRAP	繰り返して表示
	//				MIRROR	鏡写しで表示
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// 拡大縮小時の色の取得方法
	// POINT...特に指定しない
	// LINER...線形補間(デフォルト)
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	// サンプラーステートの作成
	hr = g_pDevice->CreateSamplerState(&smpDesc, &pSampler);
	if (FAILED(hr))
	{
		return hr;
	}
	// 描画にサンプラーステーとを設定
	g_pContext->PSSetSamplers(0, 1, &pSampler);
	SAFE_RELEASE(pSampler);

	// ビューポートの設定
	D3D11_VIEWPORT	vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Height = (float)height;
	vp.Width = (float)width;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pContext->RSSetViewports(1, &vp);
	

	// 初期化成功！
	return S_OK;		// 成功を表すパラメータ

}



/**
*	@概要	DirectX終了処理
*
*/
void UninitDirectX()
{
	// GPUに渡した設定をすべて解放
	g_pContext->ClearState();
	// フルスクリーン状態の解除
	g_pSwapChain->SetFullscreenState(FALSE, NULL);

	// 解放処理
	SAFE_RELEASE(g_pBlendState[0]);
	SAFE_RELEASE(g_pBlendState[1]);
	g_pContext->Release();
	g_pDevice->Release();
	g_pSwapChain->Release();
}


/**
*	@概要	描画の前処理
*
*/
void BeginDrawDirectX()
{
	// レンダーターゲットのクリア（バックバッファ？）
	float color[4] = { 0.8f,0.8f,0.9f,1.0f };
	g_pContext->ClearRenderTargetView(g_pRenderTarget, color);
}


/**
*	@概要	描画の後処理
*
*/
void EndDrawDirectX()
{
	// レンダーターゲットの内容を描画先のウィンドウに表示
	g_pSwapChain->Present(1, 0);
}


/**
*	@概要	Deviceの取得
*
*/
ID3D11Device *GetDevice()
{
	return g_pDevice;
}


/**
*	@概要	DeviceContextの取得
*
*/
ID3D11DeviceContext *GetContext()
{
	return g_pContext;
}


// αブレンド、加算合成切り替え
void SetAddBlend(bool isUse)
{
	// どちらの合成方法を使用するか
	int idx = 0;		// αブレンド
	if (isUse)
		idx = 1;		// 加算合成

	// アルファブレンドを描画に設定
	float blendFactor[4] = { 0,0,0,0 };
	g_pContext->OMSetBlendState(g_pBlendState[idx], blendFactor, 0xffffffff);

}
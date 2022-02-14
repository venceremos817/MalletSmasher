#include "Texture.h"
#include "DirectXTex/DirectXTex.h"
#include "DirectXTex/WICTextureLoader.h"

#ifdef _X86_
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/Win32/Debug/DirectXTex.lib")
#pragma comment(lib, "DirectXTex/Win32/Debug/WICTextureLoader.lib")
#else
#pragma comment(lib, "DirectXTex/Win32/Release/DirectXTex.lib")
#pragma comment(lib, "DirectXTex/Win32/Release/WICTextureLoader.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/x64/Debug/DirectXTex.lib")
#pragma comment(lib, "DirectXTex/x64/Debug/WICTextureLoader.lib")
#else
#pragma comment(lib, "DirectXTex/x64/Release/DirectXTex.lib")
#pragma comment(lib, "DirectXTex/x64/Release/WICTextureLoader.lib")
#endif
#endif

HRESULT LoadTextureFromFile(const char *pszFileName, ID3D11ShaderResourceView **ppTexture)
{
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, pszFileName, -1, wPath, MAX_PATH);
	DirectX::TexMetadata mdata;
	GetMetadataFromTGAFile(wPath, mdata);
	DirectX::ScratchImage image;
	LoadFromTGAFile(wPath, &mdata, image);

	ID3D11Device *pDevice = GetDevice();
	ID3D11DeviceContext *pContext = GetContext();

	HRESULT hr = CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), mdata, ppTexture);
	if (FAILED(hr))
	{
		ID3D11Resource *tex = NULL;
		hr = DirectX::CreateWICTextureFromFile(pDevice, pContext, wPath, &tex, ppTexture);
	}

	return hr;
}
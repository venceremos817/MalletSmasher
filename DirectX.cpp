//===== �C���N���[�h�� =====
#include "DirectX.h"


using namespace DirectX;


//===== �O���[�o���ϐ� =====
ID3D11Device			*g_pDevice;			// GPU�ɓn�����߂̃f�[�^�̍쐬���Ǘ�
ID3D11DeviceContext		*g_pContext;		// ��������̃f�[�^��GPU�ŕ`�悷�邽�߂̋��n��
IDXGISwapChain			*g_pSwapChain;		// �����̃o�b�N�o�b�t�@���Ǘ�
ID3D11RenderTargetView	*g_pRenderTarget;	// GPU�ɓn���`�����ݐ�
ID3D11BlendState		*g_pBlendState[2];	// �摜�������@�̐ݒ�f�[�^

/**
 *	@�T�v	DirectX������
 *	@����	width	�E�B���h�E����
 *			height	�E�B���h�E�c��
 *			fullScreen	�t���X�N���[���w��
 *	@�߂�l	�������̐���
 */
HRESULT InitDirextX(HWND hWnd, UINT width, UINT height, bool fullScreen)
{
	HRESULT hr;		// �֐��̎��s����

	//----- �X���b�v�`�F�[���̍쐬 -----
	// Window�̉�ʂɒ��ڕ`����s���̂ł͂Ȃ��ABackBuffer�ƌĂ΂��ڂɌ����Ȃ��̈�ɏ������݂��s���A
	// ���̃^�C�~���O��Window�̉�ʂɕ\�����s��

	// -- �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC scDesc;
	ZeroMemory(&scDesc, sizeof(scDesc));	// 0�ŏ�����
	// �o�b�N�o�b�t�@�̑傫��
	scDesc.BufferDesc.Width = width;
	scDesc.BufferDesc.Height = height;
	// �o�b�N�o�b�t�@�ň�����F�͈̔͂��w��
	// R 8bit , G 8bit , B 8bit , �� 8bit
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �K��1�w��
	scDesc.SampleDesc.Count = 1;
	// ��ʂ̍X�V�p�x(���t���b�V�����[�g)
	scDesc.BufferDesc.RefreshRate.Numerator = 1000;		// ����
	scDesc.BufferDesc.RefreshRate.Denominator = 1;		// ���q
	// �o�b�N�o�b�t�@�̎i�㉞���@���w��
	// �ŏI�I�ɉ�ʂ֏o�͂���F�̑~�����ݐ�Ƃ��Ďg�p
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// �쐬����o�b�N�o�b�t�@�̐�
	scDesc.BufferCount = 1;
	// �o�b�N�o�b�t�@�̕\����̃E�B���h�E���w��
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

	//----- �X���b�v�`�F�[���̍쐬(DirectX�̏�����)-----
	//-- �쐬�ɕK�v�ȃp�����[�^��p��

	// �h���C�o�̎��
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,	// GPU�ŕ`��(��{)
		D3D_DRIVER_TYPE_WARP,		// WARP�h���C�o�ŕ`��
		D3D_DRIVER_TYPE_REFERENCE,	// ��荂���x�i�ᑬ�j
	};
	// �h���C�o�̔z��̗v�f��
	UINT numDriver = sizeof(driverTypes) / sizeof(driverTypes[0]);

	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		// ���ꂼ��GPU���ǂ̃��x����DirectX�ɑΉ�����̂�������
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	// �@�\���x���z��̗v�f��
	UINT numFeatureLevel = sizeof(featureLevels) / sizeof(featureLevels[0]);

	//-- �X���b�v�`�F�[���̍쐬(DirectX�̏�����)
	D3D_FEATURE_LEVEL useLevel;		//���ۂɎg�p�����@�\���x��
	// �h���C�o������m�F
	for (UINT i = 0; i < numDriver; ++i)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,					// �Q�[���ł��܂�g��Ȃ� NULL��OK
			driverTypes[i],			// ���p����h���C�o
			NULL, 0,				// �Q�[���ł��܂�g��Ȃ�
			featureLevels,			// ���p����@�\���x���̈ꗗ
			numFeatureLevel,		// �@�\���x���̐�
			D3D11_SDK_VERSION,		// ���ɂǂ̃o�[�W������DirectX���g�p����̂�
			&scDesc,				// �X���b�v�`�F�[���̐ݒ�
			&g_pSwapChain,			// �쐬�����X���b�v�`�F�[���̊i�[��
			&g_pDevice,				// �쐬����Device�̊i�[��
			&useLevel,				// ���ۂɗ��p�����@�\���x��
			&g_pContext				// �쐬����Context�̊i�[��
		);

		// �쐬������������I��
		if (SUCCEEDED(hr))
			break;
	}

	// �쐬������������Ȃ�����
	if (FAILED(hr))
	{
		return E_FAIL;		// ���s��\���p�����[�^��Ԃ�
	}

	//--- �����_�[�^�[�Q�b�g�̍쐬
	// �o�b�N�o�b�t�@�̃|�C���^���擾
	ID3D11Texture2D *pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	// �o�b�N�o�b�t�@�̃|�C���^�����ƂɃ����_�[�^�[�Q�b�g�쐬
	hr = g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTarget);
	pBackBuffer->Release();
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	// �g�p���郌���_�[�^�[�Q�b�g��ݒ�
	g_pContext->OMSetRenderTargets(1, &g_pRenderTarget, NULL);


	// --- �A���t�@�u�����h(�摜�̍������@) �̐ݒ�
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	// �F	(�ŏI�I�ɕ\�������F�̌v�Z���@���w��)
	// �\�������F * �\������鑤�̓����x
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	// ���̐F * ( 1 - �\������鑤�̓����x )
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	// �\�������F �� ���̐F �̍������@ (�����Z)
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// �A���t�@
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	// �Ώ�
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// �A���t�@�u�����h�̃f�[�^�𐶐�
	hr = g_pDevice->CreateBlendState(&blendDesc, &g_pBlendState[0]);
	if (FAILED(hr))
	{
		return hr;
	}
	// ���Z�����̃f�[�^���쐬
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	// ���Z�����p�̃p�����[�^�ɂȂ��Ă���blendDesc�ŐV�����쐬
	hr = g_pDevice->CreateBlendState(&blendDesc, &g_pBlendState[1]);
	if (FAILED(hr))
	{
		return hr;
	}

	// �������@�̐ݒ�
	SetAddBlend(false);		// false�Ȃ烿�u�����h

	//--- �e�N�X�`���̌J��Ԃ��ݒ�
	// �T���v���[�X�e�[�g
	D3D11_SAMPLER_DESC smpDesc;
	ID3D11SamplerState *pSampler;
	ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
	// �e�N�X�`���̌J��Ԃ����@
	// �ق��̐ݒ�...CLAMP	�[�̐F����������(�f�t�H���g���̎w��)
	//				WRAP	�J��Ԃ��ĕ\��
	//				MIRROR	���ʂ��ŕ\��
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// �g��k�����̐F�̎擾���@
	// POINT...���Ɏw�肵�Ȃ�
	// LINER...���`���(�f�t�H���g)
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	// �T���v���[�X�e�[�g�̍쐬
	hr = g_pDevice->CreateSamplerState(&smpDesc, &pSampler);
	if (FAILED(hr))
	{
		return hr;
	}
	// �`��ɃT���v���[�X�e�[�Ƃ�ݒ�
	g_pContext->PSSetSamplers(0, 1, &pSampler);
	SAFE_RELEASE(pSampler);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT	vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Height = (float)height;
	vp.Width = (float)width;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pContext->RSSetViewports(1, &vp);
	

	// �����������I
	return S_OK;		// ������\���p�����[�^

}



/**
*	@�T�v	DirectX�I������
*
*/
void UninitDirectX()
{
	// GPU�ɓn�����ݒ�����ׂĉ��
	g_pContext->ClearState();
	// �t���X�N���[����Ԃ̉���
	g_pSwapChain->SetFullscreenState(FALSE, NULL);

	// �������
	SAFE_RELEASE(g_pBlendState[0]);
	SAFE_RELEASE(g_pBlendState[1]);
	g_pContext->Release();
	g_pDevice->Release();
	g_pSwapChain->Release();
}


/**
*	@�T�v	�`��̑O����
*
*/
void BeginDrawDirectX()
{
	// �����_�[�^�[�Q�b�g�̃N���A�i�o�b�N�o�b�t�@�H�j
	float color[4] = { 0.8f,0.8f,0.9f,1.0f };
	g_pContext->ClearRenderTargetView(g_pRenderTarget, color);
}


/**
*	@�T�v	�`��̌㏈��
*
*/
void EndDrawDirectX()
{
	// �����_�[�^�[�Q�b�g�̓��e��`���̃E�B���h�E�ɕ\��
	g_pSwapChain->Present(1, 0);
}


/**
*	@�T�v	Device�̎擾
*
*/
ID3D11Device *GetDevice()
{
	return g_pDevice;
}


/**
*	@�T�v	DeviceContext�̎擾
*
*/
ID3D11DeviceContext *GetContext()
{
	return g_pContext;
}


// ���u�����h�A���Z�����؂�ւ�
void SetAddBlend(bool isUse)
{
	// �ǂ���̍������@���g�p���邩
	int idx = 0;		// ���u�����h
	if (isUse)
		idx = 1;		// ���Z����

	// �A���t�@�u�����h��`��ɐݒ�
	float blendFactor[4] = { 0,0,0,0 };
	g_pContext->OMSetBlendState(g_pBlendState[idx], blendFactor, 0xffffffff);

}
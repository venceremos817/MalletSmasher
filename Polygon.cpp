#include "Polygon.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

//--- �萔
const unsigned char VTX_NUM = 4; // ���_��
// ���_�V�F�[�_
const char VSCode[] = "\
struct VS_IN {\
	float3 pos : POSITION0;\
	float4 color : COLOR0;\
	float2 uv : TEXCOORD0;\
};\
struct VS_OUT {\
	float4 pos : SV_POSITION;\
	float4 color : TEXCOORD0;\
	float2 uv : TEXCOORD1;\
};\
cbuffer ConstantBuffer : register(b0) {\
	float4x4 WVP;\
};\
VS_OUT main(VS_IN VIN) {\
	VS_OUT VOUT;\
	VOUT.pos = mul(float4(VIN.pos, 1), WVP);\
	VOUT.color = VIN.color;\
	VOUT.uv = VIN.uv;\
	return VOUT;\
}";
// �s�N�Z���V�F�[�_
const char PSCode[] = "\
struct PS_IN {\
	float4 pos : SV_POSITION;\
	float4 color : TEXCOORD0;\
	float2 uv : TEXCOORD1;\
};\
Texture2D tex : register(t0);\
SamplerState samp : register(s0);\
float4 main(PS_IN PIN) : SV_Target {\
	return tex.Sample(samp, PIN.uv) * PIN.color;\
}";

//--- �\����
struct Vertex
{
	float x, y, z;		// ���W
	float r, g, b, a;	// �J���[
	float u, v;			// UV
};

//--- �O���[�o���ϐ�
Vertex						g_vertex[VTX_NUM];	// ���_�f�[�^
ID3D11Buffer				*g_pBuffer;			// ���_�o�b�t�@
ID3D11ShaderResourceView	*g_pWhiteTexture;	// ��e�N�X�`��

/**
 * @brief ������
 * @param[in] width ����
 * @param[in] height �c��
 * @return ��������
 */
HRESULT InitPolygon(UINT width, UINT height)
{
	HRESULT hr = E_FAIL;
	ID3D11Device *pDevice = GetDevice();
	ID3D11DeviceContext *pContext = GetContext();

	// �����ݒ�
	ZeroMemory(g_vertex, sizeof(g_vertex));
	g_vertex[1].u = 1.0f;
	g_vertex[2].v = 1.0f;
	g_vertex[3].u = g_vertex[3].v = 1;

	// �J�����s��
	DirectX::XMMATRIX cameraMat =
		DirectX::XMMatrixOrthographicOffCenterLH(0, static_cast<float>(width), static_cast<float>(height), 0, -1, 1);
	cameraMat = DirectX::XMMatrixTranspose(cameraMat);
	DirectX::XMFLOAT4X4 f4x4;
	DirectX::XMStoreFloat4x4(&f4x4, cameraMat);


	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC vtxBufDesc;
	ZeroMemory(&vtxBufDesc, sizeof(vtxBufDesc));
	vtxBufDesc.ByteWidth = sizeof(Vertex) * VTX_NUM;
	vtxBufDesc.Usage = D3D11_USAGE_DYNAMIC;
	vtxBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vtxBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vtxSubResource;
	ZeroMemory(&vtxSubResource, sizeof(vtxSubResource));
	vtxSubResource.pSysMem = g_vertex;

	hr = pDevice->CreateBuffer(&vtxBufDesc, &vtxSubResource, &g_pBuffer);
	if (FAILED(hr)) { return hr; }

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, &g_pBuffer, &stride, &offset);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	

	// �V�F�[�_�R���p�C��
	ID3DBlob *VSobj;
	ID3DBlob *PSobj;
	ID3DBlob *error;
	hr = D3DCompile(VSCode, sizeof(VSCode), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &VSobj, &error);
	if (FAILED(hr)) { return hr; }
	hr = D3DCompile(PSCode, sizeof(PSCode), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &PSobj, &error);
	if (FAILED(hr)) { return hr; }

	//���_���C�A�E�g�쐬
	ID3D11InputLayout *pInputLayout;
	static const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,							 D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = pDevice->CreateInputLayout(layout, _countof(layout), VSobj->GetBufferPointer(), VSobj->GetBufferSize(), &pInputLayout);
	if (FAILED(hr)) { return hr; }
	pContext->IASetInputLayout(pInputLayout);
	SAFE_RELEASE(pInputLayout);

	//���_�V�F�[�_�[����
	ID3D11VertexShader *pVertexShader;
	hr = pDevice->CreateVertexShader(VSobj->GetBufferPointer(), VSobj->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr)) { return hr; }
	pContext->VSSetShader(pVertexShader, NULL, 0);
	SAFE_RELEASE(pVertexShader);
	SAFE_RELEASE(VSobj);

	D3D11_BUFFER_DESC shaderBufDesc;
	ZeroMemory(&shaderBufDesc, sizeof(shaderBufDesc));
	shaderBufDesc.ByteWidth = sizeof(f4x4);
	shaderBufDesc.Usage = D3D11_USAGE_DEFAULT;
	shaderBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA shaderSubResource;
	ZeroMemory(&shaderSubResource, sizeof(shaderSubResource));
	shaderSubResource.pSysMem = &f4x4;

	ID3D11Buffer *pShaderBuffer;
	hr = pDevice->CreateBuffer(&shaderBufDesc, &shaderSubResource, &pShaderBuffer);
	if (FAILED(hr)) { return hr; }
	pContext->VSSetConstantBuffers(0, 1, &pShaderBuffer);
	SAFE_RELEASE(pShaderBuffer);

	//�s�N�Z���V�F�[�_�[����
	ID3D11PixelShader  *pPixelShader;
	hr = pDevice->CreatePixelShader(PSobj->GetBufferPointer(), PSobj->GetBufferSize(), NULL, &pPixelShader);
	if (FAILED(hr)) { return hr; }
	pContext->PSSetShader(pPixelShader, NULL, 0);
	SAFE_RELEASE(pPixelShader);
	SAFE_RELEASE(PSobj);

	// ���F�e�N�X�`������
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = 1;
	texDesc.Height = 1;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	BYTE color[] = { 0xff, 0xff, 0xff, 0xff };
	D3D11_SUBRESOURCE_DATA texSubResource;
	ZeroMemory(&texSubResource, sizeof(texSubResource));
	texSubResource.SysMemPitch = sizeof(color);
	texSubResource.pSysMem = color;

	ID3D11Texture2D *pTex = NULL;
	hr = pDevice->CreateTexture2D(&texDesc, &texSubResource, &pTex);
	if (FAILED(hr)) { return hr; }

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderViewDesc;
	ZeroMemory(&shaderViewDesc, sizeof(shaderViewDesc));
	shaderViewDesc.Format = texDesc.Format;
	shaderViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderViewDesc.Texture2D.MipLevels = texDesc.MipLevels;

	hr = pDevice->CreateShaderResourceView(pTex, &shaderViewDesc, &g_pWhiteTexture);
	if (FAILED(hr)) { return hr; }
	SetPolygonTexture(NULL);
	SAFE_RELEASE(pTex);

	return hr;
}

/**
 * @brief �|���S���I������
 */
void UninitPolygon()
{
	SAFE_RELEASE(g_pBuffer);
}

/**
 * @brief �|���S���`��
 */
void DrawPolygon()
{
	HRESULT hr = E_FAIL;
	D3D11_MAPPED_SUBRESOURCE mapResource;
	ID3D11DeviceContext *pContext = GetContext();

	// �f�[�^�`������
	hr = pContext->Map(g_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (SUCCEEDED(hr))
	{
		memcpy_s(mapResource.pData, sizeof(g_vertex), g_vertex, sizeof(g_vertex));
	}
	pContext->Unmap(g_pBuffer, 0);

	// �`��
	pContext->Draw(VTX_NUM, 0);
}


/**
 * @brief ���_���W�ݒ�
 * @param[in] idx �ݒ肷�钸�_
 * @param[in] x X���W
 * @param[in] y Y���W
 */
void SetPolygonVtxPos(unsigned char idx, float x, float y)
{
	if (idx < VTX_NUM)
	{
		g_vertex[idx].x = x;
		g_vertex[idx].y = y;
	}
}

/**
 * @brief ���_�J���[�ݒ�
 * @param[in] idx �ݒ肷�钸�_
 * @param[in] r �ԐF
 * @param[in] g �ΐF
 * @param[in] b �F
 * @param[in] a ���ߐF
 */
void SetPolygonVtxColor(unsigned char idx, float r, float g, float b, float a)
{
	if (idx < VTX_NUM)
	{
		g_vertex[idx].r = r;
		g_vertex[idx].g = g;
		g_vertex[idx].b = b;
		g_vertex[idx].a = a;
	}
}

/**
 * @brief ���_UV�ݒ�
 * @param[in] idx �ݒ肷�钸�_
 * @param[in] u U���W
 * @param[in] v V���W
 */
void SetPolygonVtxUV(unsigned char idx, float u, float v)
{
	if (idx < VTX_NUM)
	{
		g_vertex[idx].u = u;
		g_vertex[idx].v = v;
	}
}

/**
 * @brief �e�N�X�`���\��t��
 * @param[in] pTexture �\��t����e�N�X�`��
 */
void SetPolygonTexture(ID3D11ShaderResourceView *pTexture)
{
	ID3D11DeviceContext *pContext = GetContext();
	pContext->PSSetShaderResources(0, 1, pTexture == NULL ? &g_pWhiteTexture : &pTexture);
}

// EOF
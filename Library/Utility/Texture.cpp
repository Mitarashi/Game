#include <d3d11.h>

#include "../Misc.h"
#include "../DxSystem.h"

#include "Texture.h"

void Texture::Load(const char* fileName)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	// マルチバイト文字からワイド文字へ変換
	wchar_t wfileName[256];
	::MultiByteToWideChar(CP_ACP, 0, fileName, -1, wfileName, 256);

	// 画像ファイル読み込み DirectXTex
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	HRESULT hr = DirectX::LoadFromWICFile(wfileName, DirectX::WIC_FLAGS_NONE, &metadata, image);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	hr = DirectX::CreateShaderResourceView(
		device,
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		shaderResourceView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// テクスチャデータ取得
	texture2dDesc.Width = static_cast<u_int>(metadata.width);
	texture2dDesc.Height = static_cast<u_int>(metadata.height);

	//image.Release();
}

void Texture::Set(u_int slot, bool flg)
{
	ID3D11DeviceContext* deviceContext = DxSystem::Instance().GetDeviceContext();

	if (!flg)
	{
		ID3D11ShaderResourceView*	rtv = nullptr;
		ID3D11SamplerState*			ss = nullptr;
		deviceContext->VSSetShaderResources(slot, 1, &rtv);
		deviceContext->VSSetSamplers(slot, 1, &ss);
		deviceContext->PSSetShaderResources(slot, 1, &rtv);
		deviceContext->PSSetSamplers(slot, 1, &ss);
		deviceContext->HSSetShaderResources(slot, 1, &rtv);
		deviceContext->HSSetSamplers(slot, 1, &ss);
		deviceContext->DSSetShaderResources(slot, 1, &rtv);
		deviceContext->DSSetSamplers(slot, 1, &ss);
		deviceContext->GSSetShaderResources(slot, 1, &rtv);
		deviceContext->GSSetSamplers(slot, 1, &ss);

		return;
	}

	if (shaderResourceView)
	{
		deviceContext->VSSetShaderResources(slot, 1, shaderResourceView.GetAddressOf());
		deviceContext->PSSetShaderResources(slot, 1, shaderResourceView.GetAddressOf());
		deviceContext->DSSetShaderResources(slot, 1, shaderResourceView.GetAddressOf());
		deviceContext->GSSetShaderResources(slot, 1, shaderResourceView.GetAddressOf());
		deviceContext->PSSetShaderResources(slot, 1, shaderResourceView.GetAddressOf());
	}
}

void Texture::Create(u_int width, u_int height, DXGI_FORMAT format)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d = nullptr;
	HRESULT hr = S_OK;

	// テクスチャ作成
	SecureZeroMemory(&texture2dDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texture2dDesc.Width				= width;
	texture2dDesc.Height			= height;
	texture2dDesc.MipLevels			= 1;
	texture2dDesc.ArraySize			= 1;
	texture2dDesc.Format			= format;
	texture2dDesc.SampleDesc.Count	= 1;
	texture2dDesc.Usage				= D3D11_USAGE_DEFAULT;
	texture2dDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	texture2dDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	hr = device->CreateTexture2D(&texture2dDesc, nullptr, texture2d.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	SecureZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format				= format;
	renderTargetViewDesc.ViewDimension		= D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateRenderTargetView(texture2d.Get(), &renderTargetViewDesc, renderTargetView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	SecureZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format						= format;
	shaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	shaderResourceViewDesc.Texture2D.MipLevels			= 1;
	hr = device->CreateShaderResourceView(texture2d.Get(), &shaderResourceViewDesc, shaderResourceView.GetAddressOf());

	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void Texture::CreateDepth(u_int width, u_int height, DXGI_FORMAT format)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d = nullptr;
	HRESULT hr = S_OK;

	// テクスチャ作成
	SecureZeroMemory(&texture2dDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texture2dDesc.Width					= width;
	texture2dDesc.Height				= height;
	texture2dDesc.MipLevels				= 1;
	texture2dDesc.ArraySize				= 1;
	texture2dDesc.Format				= format;
	texture2dDesc.SampleDesc.Count		= 1;
	texture2dDesc.SampleDesc.Quality	= 0;
	texture2dDesc.Usage					= D3D11_USAGE_DEFAULT;
	texture2dDesc.CPUAccessFlags		= 0;
	texture2dDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	hr = device->CreateTexture2D(&texture2dDesc, nullptr, texture2d.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// 深度ステンシルビュー設定
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	SecureZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(texture2d.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	SecureZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format						= DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	shaderResourceViewDesc.Texture2D.MipLevels			= 1;
	hr = device->CreateShaderResourceView(texture2d.Get(), &shaderResourceViewDesc, shaderResourceView.GetAddressOf());

	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void Texture::CreateCube(u_int width, u_int height, DXGI_FORMAT format, int miplevel)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D = nullptr;
	HRESULT hr = S_OK;

	//	テクスチャ作成
	SecureZeroMemory(&texture2dDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texture2dDesc.Width					= width;
	texture2dDesc.Height				= height;
	texture2dDesc.MipLevels				= miplevel;
	texture2dDesc.ArraySize				= 6;
	texture2dDesc.Format				= format;
	texture2dDesc.SampleDesc.Count		= 1;
	texture2dDesc.SampleDesc.Quality	= 0;
	texture2dDesc.Usage					= D3D11_USAGE_DEFAULT;
	texture2dDesc.CPUAccessFlags		= 0;
	texture2dDesc.BindFlags				= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture2dDesc.MiscFlags				= D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

	hr = device->CreateTexture2D(&texture2dDesc, nullptr, Texture2D.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//	シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	SecureZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceViewDesc.Format						= format;
	shaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderResourceViewDesc.TextureCube.MostDetailedMip	= 0;
	shaderResourceViewDesc.TextureCube.MipLevels		= miplevel;
	hr = device->CreateShaderResourceView(Texture2D.Get(), &shaderResourceViewDesc, shaderResourceView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//	レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	SecureZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderTargetViewDesc.Format							= format;
	renderTargetViewDesc.ViewDimension					= D3D11_RTV_DIMENSION_TEXTURE2DARRAY;//2Dテクスチャ配列
	renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
	renderTargetViewDesc.Texture2DArray.MipSlice		= 0;
	renderTargetViewDesc.Texture2DArray.ArraySize		= 6;
	hr = device->CreateRenderTargetView(Texture2D.Get(), &renderTargetViewDesc, renderTargetView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void Texture::CreateCubeDepthStencil(u_int width, u_int height)
{
	ID3D11Device* device = DxSystem::Instance().GetDevice();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D = nullptr;
	HRESULT hr = S_OK;

	//	テクスチャ作成
	SecureZeroMemory(&texture2dDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texture2dDesc.Width					= width;
	texture2dDesc.Height				= height;
	texture2dDesc.MipLevels				= 1;
	texture2dDesc.ArraySize				= 6;
	texture2dDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	texture2dDesc.SampleDesc.Count		= 1;
	texture2dDesc.SampleDesc.Quality	= 0;
	texture2dDesc.Usage					= D3D11_USAGE_DEFAULT;
	texture2dDesc.CPUAccessFlags		= 0;
	texture2dDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
	texture2dDesc.MiscFlags				= D3D11_RESOURCE_MISC_TEXTURECUBE;

	hr = device->CreateTexture2D(&texture2dDesc, nullptr, texture2D.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//	デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	SecureZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format							= DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension					= D3D11_DSV_DIMENSION_TEXTURE2DARRAY;//2Dテクスチャ配列
	depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
	depthStencilViewDesc.Texture2DArray.ArraySize		= 6;
	depthStencilViewDesc.Texture2DArray.MipSlice		= 0;
	depthStencilViewDesc.Flags							= D3D11_RESOURCE_MISC_TEXTURECUBE;
	hr = device->CreateDepthStencilView(texture2D.Get(), nullptr, depthStencilView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

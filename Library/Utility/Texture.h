#pragma once

#include <wrl.h>
#include <DirectXTex.h>

// テクスチャ
class Texture
{
public:
	Texture() : shaderResourceView(nullptr) {}
	virtual ~Texture() = default;
	void Load(const char* fileName = nullptr);

	void Set(u_int Slot, bool flg = true);

	u_int GetWidth() { return texture2dDesc.Width; }
	u_int GetHeight() { return texture2dDesc.Height; }

	void Create(u_int width, u_int height, DXGI_FORMAT format);

	// 深度ステンシルバッファ作成
	void CreateDepth(u_int width, u_int height,	DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS);

	ID3D11RenderTargetView*		GetRenderTargetView() { return renderTargetView.Get(); }
	ID3D11DepthStencilView*		GetDepthStencilView() { return depthStencilView.Get(); }
	ID3D11ShaderResourceView*	GetShaderResourceView() { return shaderResourceView.Get(); }

	// CubeMap用
	void CreateCube(u_int width, u_int height, DXGI_FORMAT format, int mipLevel = 1);
	void CreateCubeDepthStencil(u_int width, u_int height);

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	 renderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	 depthStencilView = nullptr;

	// テクスチャ情報
	D3D11_TEXTURE2D_DESC texture2dDesc = {};
};
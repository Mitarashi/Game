#pragma once

#include <d3d11.h>

#include "../Misc.h"
#include "../DxSystem.h"
#include "../ResourceManager.h"

namespace ShaderFunction
{
	using Microsoft::WRL::ComPtr;

	inline void SetSamplerState(ComPtr<ID3D11SamplerState>& samplerState, const D3D11_TEXTURE_ADDRESS_MODE address)
	{
		ID3D11Device* device = DxSystem::Instance().GetDevice();

		D3D11_SAMPLER_DESC	samplerDesc = {};
		Vec4				clearColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = address;
		samplerDesc.AddressV = address;
		samplerDesc.AddressW = address;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		memcpy(samplerDesc.BorderColor, &clearColor, sizeof(Vec4));
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;

		// サンプラーステートの生成処理
		HRESULT hr = device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 定数バッファ生成
	inline void CreateConstantBuffer(ID3D11Buffer** buffer, UINT size, D3D11_SUBRESOURCE_DATA* resource = (nullptr))
	{
		ID3D11Device* device = DxSystem::Instance().GetDevice();

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = size;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		HRESULT hr = device->CreateBuffer(&bufferDesc, resource, buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
};
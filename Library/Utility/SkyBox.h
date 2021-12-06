#pragma once

#include <wrl.h>
#include <memory>
#include <d3d11.h>

#include "../Library/Utility/Vector.h"

class SkyBox
{
public:
	SkyBox();
	~SkyBox() = default;

	void Render(const Vec2& pos, const Vec2& scale, const Vec2& texPos, const Vec2& texSize, float angle, const Vec4& color) const;

private:
	struct Vertex
	{
		Vec3 position;
		Vec2 texcoord;
		Vec4 color;
	};

	struct ConstantBufferForSkyBox
	{
		DirectX::XMFLOAT4X4 inverseView;
		DirectX::XMFLOAT4X4 inverseProjection;
		Vec4				eyePos;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				skyboxBuffer = nullptr;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView = nullptr;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout = nullptr;

	int textureWidth = 0;
	int textureHeight = 0;
};
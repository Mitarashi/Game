#pragma once

#include "Shader.h"

// LambertShaderクラス
class LambertShader : public Shader
{
public:
	LambertShader();
	~LambertShader() override = default;

	// 描画開始処理
	void Begin(const RenderContext& renderContext) override;

	// 描画処理
	void Render(const Model* model) override;

	void End() override;

private:
	struct CBufferScene
	{
		DirectX::XMFLOAT4X4 viewProjection;
		Vec4				lightDirection;
	};

	struct CBufferMesh
	{
		DirectX::XMFLOAT4X4 boneTransform[MAX_BONES];
	};

	struct CBufferSubset
	{
		Vec4 materialColor;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			meshConstantBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer = nullptr;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState = nullptr;
};
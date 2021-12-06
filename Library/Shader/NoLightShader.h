#pragma once

#include "Shader.h"

class NoLightShader : public Shader
{
public:
	NoLightShader();
	~NoLightShader() override = default;

	// �`��J�n����
	void Begin(const RenderContext& renderContext) override;

	// �`�揈��
	void Render(const Model* model) override;

	// �`��I������
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

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState = nullptr;
};
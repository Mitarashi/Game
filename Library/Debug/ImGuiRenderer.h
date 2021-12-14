#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <imgui.h>

// ImGui�����_���[
class ImGuiRenderer
{
public:
	// �R���X�g���N�^
	ImGuiRenderer(HWND hWnd);

	// �f�X�g���N�^
	~ImGuiRenderer();

	// �t���[���J�n����
	void NewFrame();

	// �`����s
	void Render();

	// WIN32���b�Z�[�W�n���h��
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	// �}�E�X�J�[�\���X�V
	bool UpdateMouseCursor();

	// �}�E�X���W�X�V
	void UpdateMousePos();

private:
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4	worldViewProjection = {};
	};

	HWND												hWnd				= {};
	INT64												time				= 0;
	INT64												ticksPerSecond		= 0;
	ImGuiMouseCursor									lastMouseCursor		= ImGuiMouseCursor_COUNT;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer		= nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				indexBuffer			= nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				constantBuffer		= nullptr;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState		= nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView	= nullptr;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader		= nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader			= nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout			= nullptr;

	int													vertexCount			= 0;
	int													indexCount			= 0;
};
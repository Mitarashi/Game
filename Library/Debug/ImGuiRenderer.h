#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <imgui.h>

// ImGuiレンダラー
class ImGuiRenderer
{
public:
	// コンストラクタ
	ImGuiRenderer(HWND hWnd);

	// デストラクタ
	~ImGuiRenderer();

	// フレーム開始処理
	void NewFrame();

	// 描画実行
	void Render();

	// WIN32メッセージハンドラ
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	// マウスカーソル更新
	bool UpdateMouseCursor();

	// マウス座標更新
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
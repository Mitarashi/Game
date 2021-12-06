#include "../Library/Misc.h"

#include "DxSystem.h"

DxSystem::DxSystem(const HWND hwnd, const bool isFullScreen) : isFullScreen(isFullScreen)
{
	HRESULT hr = S_OK;

	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(既にインスタンス化されています)");
	instance = this;

	RECT rc = {};
	GetClientRect(hwnd, &rc);
	this->screenWidth = static_cast<float>(rc.right - rc.left);
	this->screenHeight = static_cast<float>(rc.bottom - rc.top);

	// デバイス＆スワップチェーンの生成
	{
		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		// スワップチェーンを作成するための設定オプション
		DXGI_SWAP_CHAIN_DESC scd = {};
		{
			scd.BufferDesc.Width = static_cast<u_int>(screenWidth),
			scd.BufferDesc.Height = static_cast<u_int>(screenHeight),
			scd.BufferDesc.RefreshRate.Numerator = 60;
			scd.BufferDesc.RefreshRate.Denominator = 1;
			scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			scd.SampleDesc.Count = 1;
			scd.SampleDesc.Quality = 0;
			scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			scd.BufferCount = 1;
			scd.OutputWindow = hwnd;
			scd.Windowed = !isFullScreen;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			scd.Flags = 0;
		};

		D3D_FEATURE_LEVEL featureLevel = {};

		// デバイス＆スワップチェーンの生成
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&scd,
			swapChain.GetAddressOf(),
			device.GetAddressOf(),
			&featureLevel,
			deviceContext.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// レンダーターゲットビューの生成
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルビューの生成
	{
		// 深度ステンシル設定
		D3D11_TEXTURE2D_DESC tex2dDesc = {};
		tex2dDesc.Width = static_cast<u_int>(screenWidth);
		tex2dDesc.Height = static_cast<u_int>(screenHeight);
		tex2dDesc.MipLevels = 1;
		tex2dDesc.ArraySize = 1;
		tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		tex2dDesc.SampleDesc.Count = 1;
		tex2dDesc.SampleDesc.Quality = 0;
		tex2dDesc.Usage = D3D11_USAGE_DEFAULT;
		tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		tex2dDesc.CPUAccessFlags = 0;
		tex2dDesc.MiscFlags = 0;

		// 深度ステンシルテクスチャ生成
		hr = device->CreateTexture2D(
			&tex2dDesc,
			nullptr,
			depthStencilBuffer.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// 深度ステンシルビュー生成
		hr = device->CreateDepthStencilView(
			depthStencilBuffer.Get(),
			nullptr,
			depthStencilView.GetAddressOf()
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ビューポートの設定
	{
		// 画面のどの領域にDirectXで描いた画を表示するかの設定。
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = GetScreenWidth();
		viewport.Height = GetScreenHeight();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		deviceContext->RSSetViewports(1, &viewport);
	}

	CreateDepthStencilState();
	CreateRasterizerState();
	CreateBlendState();


	// レンダラ
	{
		debugRenderer = std::make_unique<DebugRenderer>();
		imGuiRenderer = std::make_unique<ImGuiRenderer>(hwnd);
	}
}

void DxSystem::Clear(float r, float g, float b, float a) const
{
	FLOAT color[] = { r, g, b, a };	// RGBA(0.0～1.0)
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
}

void DxSystem::CreateDepthStencilState()
{
	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_DESC dsd = {};

	// FALSE
	{
		dsd.DepthEnable = false;
		hr = device->CreateDepthStencilState(&dsd, depthStencilState[GetNum(DS::FALSE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// TRUE
	{
		dsd.DepthEnable = true;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;
		hr = device->CreateDepthStencilState(&dsd, depthStencilState[GetNum(DS::TRUE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// WRITE_FALSE
	{
		dsd.DepthEnable = true;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;
		hr = device->CreateDepthStencilState(&dsd, depthStencilState[GetNum(DS::WRITE_FALSE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

void DxSystem::CreateRasterizerState()
{
	HRESULT hr = S_OK;

	D3D11_RASTERIZER_DESC rd = {};
	rd.FrontCounterClockwise = false;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = false;
	rd.AntialiasedLineEnable = false;

	// CULL_BACK
	{
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;

		hr = device->CreateRasterizerState(&rd, rasterizerState[GetNum(RS::CULL_BACK)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// CULL_NONE
	{
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_NONE;

		hr = device->CreateRasterizerState(&rd, rasterizerState[GetNum(RS::CULL_NONE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// CULL_FRONT
	{
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_FRONT;
		rd.AntialiasedLineEnable = false;

		hr = device->CreateRasterizerState(&rd, rasterizerState[GetNum(RS::CULL_FRONT)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// WIREFRAME
	{
		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.CullMode = D3D11_CULL_BACK;

		hr = device->CreateRasterizerState(&rd, rasterizerState[GetNum(RS::WIREFRAME)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

void DxSystem::CreateBlendState()
{
	HRESULT hr = S_OK;

	D3D11_BLEND_DESC bd = {};

	// NONE
	{
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = false;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&bd, blendState[GetNum(BS::NONE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ALPHA
	{
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&bd, blendState[GetNum(BS::ALPHA)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ADD
	{
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&bd, blendState[GetNum(BS::ADD)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// SUBTRACT
	{
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&bd, blendState[GetNum(BS::SUBTRACT)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// REPLACE
	{
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&bd, blendState[GetNum(BS::REPLACE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// MULTIPLY
	{
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&bd, blendState[GetNum(BS::MULTIPLY)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// LIGHTEN
	{
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&bd, blendState[GetNum(BS::LIGHTEN)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// DARKEN
	{
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&bd, blendState[GetNum(BS::DARKEN)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// SCREEN
	{
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = device->CreateBlendState(&bd, blendState[GetNum(BS::SCREEN)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}
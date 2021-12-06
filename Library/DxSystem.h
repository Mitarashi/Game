#pragma once

#include <mutex>
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>

#include "Misc.h"
#include "Debug/ImGuiRenderer.h"
#include "Debug/DebugRenderer.h"

class DxSystem
{
public:
	enum class DS
	{
		FALSE = 0,
		TRUE,
		WRITE_FALSE
	};

	enum class RS
	{
		CULL_BACK = 0,
		CULL_NONE,
		CULL_FRONT,
		WIREFRAME
	};

	enum class BS
	{
		NONE = 0,
		ALPHA,
		ADD,
		SUBTRACT,
		REPLACE,
		MULTIPLY,
		LIGHTEN,
		DARKEN,
		SCREEN
	};

public:
	// コンストラクタ
	const DxSystem(const HWND	hwnd, const bool isFullScreen = (false));

	// デストラクタ
	~DxSystem() = default;

	// インスタンス取得
	static DxSystem& Instance() { return *instance; }

	// 画面クリア
	void Clear(Vec3 color) const { Clear(color.x, color.y, color.z); }
	void Clear(Vec4 color) const { Clear(color.x, color.y, color.z, color.w); }
	void Clear(float r, float g, float b, float a = (1.0f)) const;

	// getter
	ID3D11Device*				GetDevice()						const { return device.Get(); }
	ID3D11DeviceContext*		GetDeviceContext()				const { return deviceContext.Get(); }
	ID3D11RenderTargetView*		GetRenderTargetView()			const { return renderTargetView.Get(); }
	ID3D11DepthStencilView*		GetDepthStencilView()			const { return depthStencilView.Get(); }
	IDXGISwapChain*				GetSwapChain()					const { return swapChain.Get(); }
	ID3D11DepthStencilState*	GetDepthStencilState(DS state)	const { return depthStencilState[GetNum(state)].Get(); }
	ID3D11RasterizerState*		GetRasterizerState(RS state)	const { return rasterizerState[GetNum(state)].Get(); }
	ID3D11BlendState*			GetBlendState(BS state)			const { return blendState[GetNum(state)].Get(); }
	ImGuiRenderer*				GetImGuiRenderer()				const { return imGuiRenderer.get(); }
	DebugRenderer*				GetDebugRenderer()				const { return debugRenderer.get(); }
	std::mutex&					GetMutex()							  { return mutex; }
	const float					GetDeltaTime()					const { return deltaTime; }
	const float					GetScreenWidth()				const { return screenWidth; }
	const float					GetScreenHeight()				const { return screenHeight; }
	const bool					GetIsFullScreen()				const { return isFullScreen; }

	// setter
	void						SetDeltaTime(float deltaTime_) { deltaTime = deltaTime_; }

private:
	// デプスステンシルステート作成
	void CreateDepthStencilState();

	// ラスタライザステート作成
	void CreateRasterizerState();

	// ブレンドステート作成
	void CreateBlendState();

private:
	static const int DEPTH_STENCIL_TYPE = 3;
	static const int RASTERIZER_TYPE	= 4;
	static const int BLEND_TYPE			= 9;

private:
	inline static DxSystem* instance = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Device>			device				= nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		deviceContext		= nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView	= nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depthStencilBuffer	= nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  depthStencilView	= nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapChain			= nullptr;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState[DEPTH_STENCIL_TYPE]	= { nullptr };
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState[RASTERIZER_TYPE]		= { nullptr };
	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState[BLEND_TYPE]					= { nullptr };

	std::unique_ptr<ImGuiRenderer>					imGuiRenderer		= nullptr;
	std::unique_ptr<DebugRenderer>					debugRenderer		= nullptr;

	std::mutex										mutex				= {};

	float											deltaTime			= 0.0f;

	float											screenWidth			= 0.0f;
	float											screenHeight		= 0.0f;

	bool											isFullScreen		= false;
};
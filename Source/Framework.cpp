#include <memory>
#include <sstream>

#include "Scene/SceneTitle.h"

#include "Framework.h"

// コンストラクタ
Framework::Framework(HWND hWnd)	: hWnd(hWnd), audio(), inputManager(hWnd), dxSystem(hWnd)
{
	SceneManager::Create();

	// シーン初期化
	SceneManager::Instance().ChangeScene(std::make_unique<SceneTitle>());
}

// デストラクタ
Framework::~Framework()
{
	// シーン終了化
	SceneManager::Destroy();
}

// 更新処理
void Framework::Update()
{
	inputManager.Update();

	// シーン更新処理
	SceneManager::Instance().Update();
}

// 描画処理
void Framework::Render()
{
	std::lock_guard<std::mutex> lock(dxSystem.GetMutex());

	// IMGUIフレーム開始処理
	dxSystem.GetImGuiRenderer()->NewFrame();

	// シーン描画処理
	SceneManager::Instance().Render();

	// ImGui描画処理
	SceneManager::Instance().OnGui();

	// IMGUI描画
	dxSystem.GetImGuiRenderer()->Render();

	// バックバッファに描画した画を画面に表示する。
	dxSystem.GetSwapChain()->Present(syncInterval, 0);
}

// フレームレート計算
void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the
	// average time it takes to render one frame.  These stats
	// are appended to the window caption bar.
	static int frames = 0;
	static float timeTlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((highResolutionTimer.TimeStamp() - timeTlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hWnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		timeTlapsed += 1.0f;
	}
}

// アプリケーションループ
int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			highResolutionTimer.Tick();
			CalculateFrameStats();
			float elapsedTime = syncInterval == 0 ? highResolutionTimer.TimeInterval() : syncInterval / 60.0f;

			dxSystem.SetDeltaTime(elapsedTime);
			Update();
			Render();
		}
	}
	return static_cast<int>(msg.wParam);
}

// メッセージハンドラ
LRESULT CALLBACK Framework::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (dxSystem.GetImGuiRenderer()->HandleMessage(hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		highResolutionTimer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		highResolutionTimer.Start();
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

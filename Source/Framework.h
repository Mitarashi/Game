#pragma once

#include <windows.h>

#include "../Library/DxSystem.h"
#include "../Library/Audio/Audio.h"
#include "../Library/Input/InputManager.h"
#include "../Library/Debug/HighResolutionTimer.h"

#include "Scene/SceneManager.h"

// フレームワーク
class Framework
{
public:
	// コンストラクタ
	Framework(HWND hWnd);

	// デストラクタ
	~Framework();

	// アプリケーションループ
	int Run();

	// メッセージハンドラ
	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	// 更新処理
	void Update();

	// 描画処理
	void Render();

	// フレームレート計算
	void CalculateFrameStats();

private:
	// 垂直同期間隔設定
#if defined(DEBUG) | defined(_DEBUG)
	static const int syncInterval = 0;
#else
	static const int syncInterval = 1;
#endif

private:
	const HWND				hWnd;
	Audio					audio;
	DxSystem				dxSystem;
	InputManager			inputManager;
	HighResolutionTimer		highResolutionTimer;
};
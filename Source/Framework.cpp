#include <memory>
#include <sstream>

#include "Scene/SceneTitle.h"

#include "Framework.h"

// �R���X�g���N�^
Framework::Framework(HWND hWnd)	: hWnd(hWnd), audio(), inputManager(hWnd), dxSystem(hWnd)
{
	SceneManager::Create();

	// �V�[��������
	SceneManager::Instance().ChangeScene(std::make_unique<SceneTitle>());
}

// �f�X�g���N�^
Framework::~Framework()
{
	// �V�[���I����
	SceneManager::Destroy();
}

// �X�V����
void Framework::Update()
{
	inputManager.Update();

	// �V�[���X�V����
	SceneManager::Instance().Update();
}

// �`�揈��
void Framework::Render()
{
	std::lock_guard<std::mutex> lock(dxSystem.GetMutex());

	// IMGUI�t���[���J�n����
	dxSystem.GetImGuiRenderer()->NewFrame();

	// �V�[���`�揈��
	SceneManager::Instance().Render();

	// ImGui�`�揈��
	SceneManager::Instance().OnGui();

	// IMGUI�`��
	dxSystem.GetImGuiRenderer()->Render();

	// �o�b�N�o�b�t�@�ɕ`�悵�������ʂɕ\������B
	dxSystem.GetSwapChain()->Present(syncInterval, 0);
}

// �t���[�����[�g�v�Z
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

// �A�v���P�[�V�������[�v
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

// ���b�Z�[�W�n���h��
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

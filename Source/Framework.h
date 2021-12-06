#pragma once

#include <windows.h>

#include "../Library/DxSystem.h"
#include "../Library/Audio/Audio.h"
#include "../Library/Input/InputManager.h"
#include "../Library/Debug/HighResolutionTimer.h"

#include "Scene/SceneManager.h"

// �t���[�����[�N
class Framework
{
public:
	// �R���X�g���N�^
	Framework(HWND hWnd);

	// �f�X�g���N�^
	~Framework();

	// �A�v���P�[�V�������[�v
	int Run();

	// ���b�Z�[�W�n���h��
	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	// �X�V����
	void Update();

	// �`�揈��
	void Render();

	// �t���[�����[�g�v�Z
	void CalculateFrameStats();

private:
	// ���������Ԋu�ݒ�
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
#pragma once

#include "GamePad.h"
#include "Mouse.h"

// �C���v�b�g�}�l�[�W���[
class InputManager
{
public:
	// �R���X�g���N�^
	InputManager(HWND hWnd);

	// �f�X�g���N�^
	~InputManager() = default;

public:
	// �C���X�^���X�擾
	static InputManager& Instance() { return *instance; }

	// �X�V����
	void Update();

	// �Q�[���p�b�h�擾
	GamePad& GetGamePad() { return gamePad; }

	// �}�E�X�擾
	Mouse& GetMouse() { return mouse; }

	// �f�o�b�O�pGUI�`��
	void DebugGUI();

private:
	int GetInput(GamePadButton& button, GamePadButton BTN) { return (button & BTN) / BTN; }

private:
	inline static InputManager*	instance = nullptr;
	GamePad						gamePad;
	Mouse						mouse;
};

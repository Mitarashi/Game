#pragma once

#include "Obj3d.h"

// �{�b�N�X
class Box : public Obj3d
{
public:
	// �R���X�g���N�^
	Box();

	// �f�X�g���N�^
	~Box() override = default;

	// �X�V����
	void Update();

	// �`�揈��
	void Render(const RenderContext& renderContext);

	// �f�o�b�O�pGUI�`��
	void DebugGUI();

	// �f�o�b�O�v���~�e�B�u�`��
	void DebugPrimitive();
};
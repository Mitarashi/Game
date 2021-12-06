#pragma once

#include <d3d11.h>
#include <string>

#include "../Library/Shader/RenderContext.h"

#include "Character.h"

// �G�l�~�[
class Enemy : public Character, public std::enable_shared_from_this<Enemy>
{
public:
	// �R���X�g���N�^
	Enemy() = default;

	// �f�X�g���N�^
	~Enemy() = default;

	// �X�V����
	virtual void Update() = 0;

	// �`�揈��
	virtual void Render(const RenderContext& renderContext) = 0;

	// �폜����
	void Destroy();

	// �f�o�b�O�pGUI�`��
	virtual void DebugGUI() = 0;

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DebugPrimitive();

protected:
	std::string name = {};
};
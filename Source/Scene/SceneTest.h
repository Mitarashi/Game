#pragma once

#include "Scene.h"

// �^�C�g���V�[��
class SceneTest : public Scene
{
public:
	// �R���X�g���N�^
	SceneTest() = default;

	// �f�X�g���N�^
	virtual ~SceneTest() override = default;

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update() override;

	// �`�揈��
	void Render() override;

	// ImGui�`��
	void OnGui() override;

private:
};
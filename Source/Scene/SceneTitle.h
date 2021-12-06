#pragma once

#include "../Library/Utility/Sprite.h"

#include "Scene.h"

// �^�C�g���V�[��
class SceneTitle : public Scene
{
public:
	// �R���X�g���N�^
	SceneTitle() = default;

	// �f�X�g���N�^
	virtual ~SceneTitle() override = default;

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
	std::unique_ptr<Sprite> titleSpr = nullptr;
};
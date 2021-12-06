#pragma once

#include <memory>

#include "../Library/Utility/Sprite.h"

#include "../Source/Scene/Scene.h"

// ���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
	// �R���X�g���N�^
	SceneLoading() = default;
	SceneLoading(std::unique_ptr<Scene> nextScene) : nextScene(std::move(nextScene)) {};

	// �f�X�g���N�^
	~SceneLoading() override = default;

	// ����������
	void Initialize() override;

	// �I��������
	void Finalize() override;

	// �X�V����
	void Update() override;

	// �`�揈��
	void Render() override;

	// ImGui�`��
	void OnGui() override;

private:
	// ���[�f�B���O�X���b�h
	static void LoadingThread(SceneLoading* scene);

private:
	std::unique_ptr<Sprite> sprite = nullptr;
	float					angle = 0.0f;
	std::unique_ptr<Scene>	nextScene = nullptr;
};
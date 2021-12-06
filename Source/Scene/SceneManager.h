#pragma once

#include <memory>

// �O���錾
class Scene;

// �V�[���}�l�[�W���[
class SceneManager
{
private:
	// �R���X�g���N�^
	SceneManager() = default;

	// �f�X�g���N�^
	~SceneManager() = default;

public:
	// �R���X�g���N�^
	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;

	// �I�y���[�^�[
	SceneManager& operator= (const SceneManager&) = delete;
	SceneManager& operator= (SceneManager&&) = delete;

	// �C���X�^���X�쐬
	static void Create();

	// �C���X�^���X�폜
	static void Destroy();

	// �C���X�^���X�擾
	static SceneManager& Instance() { return *instance; }

	// �X�V����
	void Update();

	// �`�揈��
	void Render();

	// ImGui�`��
	void OnGui();

	// �V�[���N���A
	void Clear();

	// �V�[���؂�ւ�
	void ChangeScene(std::unique_ptr<Scene> scene);

private:
	inline static SceneManager* instance = nullptr;
	std::unique_ptr<Scene> currentScene = nullptr;
};
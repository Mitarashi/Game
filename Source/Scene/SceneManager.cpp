#include "Scene.h"
#include "SceneManager.h"

// �C���X�^���X�쐬
void SceneManager::Create()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(���ɃC���X�^���X������Ă��܂�)");
	instance = new SceneManager;
}

// �C���X�^���X�폜
void SceneManager::Destroy()
{
	if (instance != nullptr)
	{
		instance->Clear();
		delete instance;
		instance = nullptr;
	}
}

// �X�V����
void SceneManager::Update()
{
	if (currentScene != nullptr)
	{
		currentScene->Update();
	}
}

// �`�揈��
void SceneManager::Render()
{
	if (currentScene != nullptr)
	{
		currentScene->Render();
	}
}

// ImGui�`��
void SceneManager::OnGui()
{
	if (currentScene != nullptr)
	{
		currentScene->OnGui();
	}
}

// �V�[���N���A
void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		currentScene.reset();
		currentScene = nullptr;
	}
}

// �V�[���؂�ւ�
void SceneManager::ChangeScene(std::unique_ptr<Scene> scene)
{
	// ���݂̃V�[���̏I������
	Clear();

	// �V�����V�[����ݒ�
	currentScene = std::move(scene);

	if (!currentScene->IsReady())
	{
		currentScene->Initialize();
	}
}
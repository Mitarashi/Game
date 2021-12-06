#include <thread>

#include "../Library/Math.h"
#include "../Library/DxSystem.h"
#include "../Library/Input/InputManager.h"

#include "../Source/Scene/SceneManager.h"

#include "SceneLoading.h"

void SceneLoading::Initialize()
{
	sceneName = "Loading";

	sprite = std::make_unique<Sprite>("Data/Sprite/LoadingIcon.png");

	std::thread thread(LoadingThread, this);

	thread.detach();
}

// �I����
void SceneLoading::Finalize()
{
}

// �X�V����
void SceneLoading::Update()
{
	constexpr float speed = 180;
	angle += speed * DxSystem::Instance().GetDeltaTime();

	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(std::move(nextScene));
		return;
	}
}

// �`�揈��
void SceneLoading::Render()
{
	DxSystem& dxSystem = DxSystem::Instance();

	Vec4 clearColor = { 0.0f, 0.0f, 0.5f, 1.0f };
	dxSystem.Clear(clearColor);

	// 2D�X�v���C�g�`��
	{
		float screenWidth = static_cast<float>(dxSystem.GetScreenWidth());
		float screenHeight = static_cast<float>(dxSystem.GetScreenHeight());
		float textureWidth = static_cast<float>(sprite->GetTextureWidth());
		float textureHeight = static_cast<float>(sprite->GetTextureHeight());
		float positionX = screenWidth - textureWidth;
		float positionY = screenHeight - textureHeight;

		sprite->Render(positionX, positionY, textureWidth, textureHeight,
			0, 0, textureWidth, textureHeight,
			angle,
			1, 1, 1, 1);
	}
}

// ���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
	// COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
	CoInitialize(nullptr);

	// ���̃V�[���̏��������s��
	scene->nextScene->Initialize();

	// �X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	// ���̃V�[���̏��������ݒ�
	scene->nextScene->SetReady();
}

void SceneLoading::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(900, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin(sceneName.c_str(), nullptr, ImGuiWindowFlags_None))
	{
	}
	ImGui::End();
}
#include "../Library/DxSystem.h"
#include "../Library/Audio/Audio.h"
#include "../Library/Input/InputManager.h"

#include "Character/EnemySlime.h"
#include "Character/EnemyMutant.h"

#include "../Stage/StageMain.h"
#include "SceneGame.h"

// ������
void SceneGame::Initialize()
{
	sceneName = "Game";

	EnemyManager::Create();
	StageManager::Create();
	CameraManager::Create();
	PlayerManager::Create();

	DxSystem& dxSystem = DxSystem::Instance();

	// �J����������
	{
		CameraManager::Instance().Initialize();
	}

	// �X�e�[�W������
	{
		std::shared_ptr<Stage> stage = std::make_shared<StageMain>();
		StageManager::Instance().Register(stage);
	}

	// �v���C���[������
	{
		PlayerManager::Instance().Initialize();
	}

	// �G�l�~�[������
	{
		//for (int i = 0; i < 1; ++i)
		//{
		//	std::shared_ptr<EnemySlime> slime = std::make_shared<EnemySlime>(device);
		//	slime->SetPosition(VEC3(i * 2, 0, 5));
		//	slime->SetTerritory(slime->GetPosition(), 10.0f);
		//	enemyManager.Register(slime);
		//}
		std::shared_ptr<EnemyMutant> mutant = std::make_shared<EnemyMutant>();
		mutant->SetPosition(Vec3(4, 0, 5));
		EnemyManager::Instance().Register(mutant);
	}

	// �X�J�C�{�b�N�X������
	{
		skyBox = std::make_unique<SkyBox>();
	}
}

// �I����
void SceneGame::Finalize()
{
	StageManager::Destory();
	EnemyManager::Destroy();
	PlayerManager::Destroy();
	CameraManager::Destroy();
}

// �X�V����
void SceneGame::Update()
{
	// �J�����X�V����
	{
		Vec3 target = PlayerManager::Instance().GetPlayer()->GetPosition();
		target.y += 0.5f;
		CameraManager::Instance().SetTarget(target);
		CameraManager::Instance().Update();
	}

	StageManager::Instance().Update();
	EnemyManager::Instance().Update();
	PlayerManager::Instance().Update();
}

// �`�揈��
void SceneGame::Render()
{
	DxSystem& dxSystem = DxSystem::Instance();

	Vec4 clearColor = { 0.0f, 0.0f, 0.5f, 1.0f };
	dxSystem.Clear(clearColor);

	// �`�揈��
	Vec4 lightDirection = { 0.0f,-1.0f,0.0f,0.0f };
	RenderContext rc = {};
	rc.lightDirection = lightDirection;

	// �J�����p�����[�^�ݒ�
	{
		rc.view = CameraManager::Instance().GetMainCamera()->GetView();
		rc.projection = CameraManager::Instance().GetMainCamera()->GetProjection();
	}

	skyBox->Render({ 0,0 }, { dxSystem.GetScreenWidth(),dxSystem.GetScreenHeight() }, { 0,0 }, { 8192,4096 }, 0, { 1,1,1,1 });

	// 3D���f���`��
	{
		StageManager::Instance().Render(rc);
		EnemyManager::Instance().Render(rc);
		PlayerManager::Instance().Render(rc);
	}

	// 3D�G�t�F�N�g�`��
	{
	}

	// 3D�f�o�b�O�`��
	{
		if (debug)
		{
			EnemyManager::Instance().DebugPrimitive();
			PlayerManager::Instance().DebugPrimitive();
			dxSystem.GetDebugRenderer()->DrawCylinder(spawnPos, spawnRange, 5.0f, Vec4(0, 0, 1, 1));
			dxSystem.GetDebugRenderer()->Render(rc);
		}
	}

	// 2D�X�v���C�g�`��
	{
		//sprite->Render(deviceContext, 0, 0, 1280, 720, 0, 0, sprite->GetTextureWidth(), sprite->GetTextureHeight(), 0);
	}
}

// ImGui�`��
void SceneGame::OnGui()
{
	ImGui::SetNextWindowPos(ImVec2(900, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin(sceneName.c_str(), nullptr, ImGuiWindowFlags_None))
	{
		ImGui::Checkbox("Debug", &debug);
		ImGui::InputFloat3("SpawnPosition", &spawnPos.x);
		ImGui::InputFloat("SpawnRange", &spawnRange);
		if (ImGui::Button("EnemyPop"))
		{
			std::shared_ptr<EnemySlime> slime = std::make_shared<EnemySlime>();
			Vec3 pos = { rand() % static_cast<int>(spawnRange * 2) - spawnRange,
			0, rand() % static_cast<int>(spawnRange * 2) - spawnRange };
			slime->SetPosition(pos);
			slime->SetTerritory(slime->GetPosition(), 10.0f);
			EnemyManager::Instance().Register(slime);
		}
	}
	ImGui::End();

	if (debug)
	{
		EnemyManager::Instance().DebugGUI();
		PlayerManager::Instance().DebugGUI();
		InputManager::Instance().DebugGUI();
	}
	Scene::OnGui();
}
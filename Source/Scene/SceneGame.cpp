#include "../Library/DxSystem.h"
#include "../Library/Audio/Audio.h"
#include "../Library/Input/InputManager.h"

#include "Character/EnemySlime.h"
#include "Character/EnemyMutant.h"

#include "../Stage/StageMain.h"
#include "SceneGame.h"

// 初期化
void SceneGame::Initialize()
{
	sceneName = "Game";

	EnemyManager::Create();
	StageManager::Create();
	CameraManager::Create();
	PlayerManager::Create();

	DxSystem& dxSystem = DxSystem::Instance();

	// カメラ初期化
	{
		CameraManager::Instance().Initialize();
	}

	// ステージ初期化
	{
		std::shared_ptr<Stage> stage = std::make_shared<StageMain>();
		StageManager::Instance().Register(stage);
	}

	// プレイヤー初期化
	{
		PlayerManager::Instance().Initialize();
	}

	// エネミー初期化
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

	// スカイボックス初期化
	{
		skyBox = std::make_unique<SkyBox>();
	}
}

// 終了化
void SceneGame::Finalize()
{
	StageManager::Destory();
	EnemyManager::Destroy();
	PlayerManager::Destroy();
	CameraManager::Destroy();
}

// 更新処理
void SceneGame::Update()
{
	// カメラ更新処理
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

// 描画処理
void SceneGame::Render()
{
	DxSystem& dxSystem = DxSystem::Instance();

	Vec4 clearColor = { 0.0f, 0.0f, 0.5f, 1.0f };
	dxSystem.Clear(clearColor);

	// 描画処理
	Vec4 lightDirection = { 0.0f,-1.0f,0.0f,0.0f };
	RenderContext rc = {};
	rc.lightDirection = lightDirection;

	// カメラパラメータ設定
	{
		rc.view = CameraManager::Instance().GetMainCamera()->GetView();
		rc.projection = CameraManager::Instance().GetMainCamera()->GetProjection();
	}

	skyBox->Render({ 0,0 }, { dxSystem.GetScreenWidth(),dxSystem.GetScreenHeight() }, { 0,0 }, { 8192,4096 }, 0, { 1,1,1,1 });

	// 3Dモデル描画
	{
		StageManager::Instance().Render(rc);
		EnemyManager::Instance().Render(rc);
		PlayerManager::Instance().Render(rc);
	}

	// 3Dエフェクト描画
	{
	}

	// 3Dデバッグ描画
	{
		if (debug)
		{
			EnemyManager::Instance().DebugPrimitive();
			PlayerManager::Instance().DebugPrimitive();
			dxSystem.GetDebugRenderer()->DrawCylinder(spawnPos, spawnRange, 5.0f, Vec4(0, 0, 1, 1));
			dxSystem.GetDebugRenderer()->Render(rc);
		}
	}

	// 2Dスプライト描画
	{
		//sprite->Render(deviceContext, 0, 0, 1280, 720, 0, 0, sprite->GetTextureWidth(), sprite->GetTextureHeight(), 0);
	}
}

// ImGui描画
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
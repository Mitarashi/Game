#include "../Library/DxSystem.h"

#include "Collision.h"
#include "EnemyManager.h"

// インスタンス作成
void EnemyManager::Create()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(既にインスタンス化されています)");
	instance = new EnemyManager;
}

// インスタンス削除
void EnemyManager::Destroy()
{
	if (instance != nullptr)
	{
		instance->Clear();
		delete instance;
		instance = nullptr;
	}
}

// 更新処理
void EnemyManager::Update()
{
	for (std::shared_ptr<Enemy>& enemy : enemies)
	{
		enemy->Update();
	}

	for (std::shared_ptr<Enemy>& enemy : removes)
	{
		auto it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end()) enemies.erase(it);
	}

	//CollisionEnemyVsEnemy();
}

// 描画処理
void EnemyManager::Render(const RenderContext& rc)
{
	for (std::shared_ptr<Enemy>& enemy : enemies)
	{
		enemy->Render(rc);
	}
}

// エネミー全削除
void EnemyManager::Clear()
{
	for (std::shared_ptr<Enemy>& enemy : enemies)
	{
		auto it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end()) enemies.erase(it);
	}

	for (std::shared_ptr<Enemy>& enemy : removes)
	{
		auto it = std::find(removes.begin(), removes.end(), enemy);
		if (it != removes.end()) removes.erase(it);
	}
}

// デバッグ用GUI描画
void EnemyManager::DebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Enemy", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::Text("EnemyCount %d", static_cast<int>(enemies.size()));

		for (std::shared_ptr<Enemy>& enemy : enemies)
		{
			enemy->DebugGUI();
		}
	}
	ImGui::End();
}

// デバッグプリミティブ描画
void EnemyManager::DebugPrimitive()
{
	for (std::shared_ptr<Enemy>& enemy : enemies)
	{
		enemy->DebugPrimitive();
	}
}

// エネミー同士の当たり判定
void EnemyManager::CollisionEnemyVsEnemy()
{
	int enemyCount = GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		auto enemy1 = GetEnemy(i);

		for (int j = 0; j < enemyCount; ++j)
		{
			auto enemy2 = GetEnemy(j);
			if (enemy1 == enemy2)continue;

			JudgeCylinder c1(*enemy1);
			JudgeCylinder c2(*enemy2);
			Vec3 outPosition2 = {};
			if (Collision::IntersectCylinderVsCylinder(c1, c2, outPosition2))
			{
				enemy2->SetPosition(outPosition2);
			}

			Vec3 outPosition1 = {};
			if (Collision::IntersectCylinderVsCylinder(c2, c1, outPosition1))
			{
				enemy1->SetPosition(outPosition1);
			}
		}
	}
}

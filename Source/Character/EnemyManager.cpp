#include "../Library/DxSystem.h"

#include "Collision.h"
#include "EnemyManager.h"

// �C���X�^���X�쐬
void EnemyManager::Create()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(���ɃC���X�^���X������Ă��܂�)");
	instance = new EnemyManager;
}

// �C���X�^���X�폜
void EnemyManager::Destroy()
{
	if (instance != nullptr)
	{
		instance->Clear();
		delete instance;
		instance = nullptr;
	}
}

// �X�V����
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

// �`�揈��
void EnemyManager::Render(const RenderContext& rc)
{
	for (std::shared_ptr<Enemy>& enemy : enemies)
	{
		enemy->Render(rc);
	}
}

// �G�l�~�[�S�폜
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

// �f�o�b�O�pGUI�`��
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

// �f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DebugPrimitive()
{
	for (std::shared_ptr<Enemy>& enemy : enemies)
	{
		enemy->DebugPrimitive();
	}
}

// �G�l�~�[���m�̓����蔻��
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

#pragma once

#include <vector>
#include <memory>

#include "../Library/Shader/RenderContext.h"

#include "../Character/Enemy.h"

// �G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
	// �R���X�g���N�^
	EnemyManager() = default;

	// �f�X�g���N�^
	~EnemyManager() = default;

public:
	// �R���X�g���N�^
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager(EnemyManager&&) = delete;

	// �I�y���[�^�[
	EnemyManager& operator= (const EnemyManager&) = delete;
	EnemyManager& operator= (EnemyManager&&) = delete;

	// �C���X�^���X�쐬
	static void Create();

	// �C���X�^���X�폜
	static void Destroy();

	// �C���X�^���X�擾
	static EnemyManager& Instance() { return *instance; }

	// �X�V����
	void Update();

	// �`�揈��
	void Render(const RenderContext& renderContext);

	// �G�l�~�[�o�^
	void Register(std::shared_ptr<Enemy> enemy) { enemies.emplace_back(enemy); }

	// �G�l�~�[�폜
	void Remove(std::shared_ptr<Enemy> enemy) { removes.emplace_back(enemy); }

	// �G�l�~�[�S�폜
	void Clear();

	// �f�o�b�O�pGUI�`��
	void DebugGUI();

	// �f�o�b�O�v���~�e�B�u�`��
	void DebugPrimitive();

	// �G�l�~�[�̐��擾
	const int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// �G�l�~�[�擾
	std::shared_ptr<Enemy> GetEnemy(int index)
	{
		if (enemies.size() <= index) return nullptr;
		else return enemies.at(index);
	}

private:
	// �G�l�~�[���m�̓����蔻��
	void CollisionEnemyVsEnemy();

private:
	inline static EnemyManager* instance = nullptr;
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::shared_ptr<Enemy>> removes;
};
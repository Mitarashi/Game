#include "../Library/DxSystem.h"

#include "EnemyManager.h"
#include "Enemy.h"

// �폜����
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(shared_from_this());
}

// �f�o�b�O�v���~�e�B�u�`��
void Enemy::DebugPrimitive()
{
	DebugRenderer* debugRenderer = DxSystem::Instance().GetDebugRenderer();

	debugRenderer->DrawCylinder(position, radius, height, Vec4(1, 0, 0, 1));
}
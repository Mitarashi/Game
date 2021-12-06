#include "../Library/DxSystem.h"

#include "EnemyManager.h"
#include "Enemy.h"

// 削除処理
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(shared_from_this());
}

// デバッグプリミティブ描画
void Enemy::DebugPrimitive()
{
	DebugRenderer* debugRenderer = DxSystem::Instance().GetDebugRenderer();

	debugRenderer->DrawCylinder(position, radius, height, Vec4(1, 0, 0, 1));
}
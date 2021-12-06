#include "Stage/StageManager.h"

// インスタンス作成
void StageManager::Create()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(既にインスタンス化されています)");
	instance = new StageManager;
}

// インスタンス削除
void StageManager::Destory()
{
	if (instance != nullptr)
	{
		instance->Clear();
		delete instance;
		instance = nullptr;
	}
}

// 更新処理
void StageManager::Update()
{
	for (auto& stage : stages)
	{
		stage->Update();
	}
}

// 描画処理
void StageManager::Render(const RenderContext& rc)
{
	for (auto& stage : stages)
	{
		stage->Render(rc);
	}
}

// ステージ全削除
void StageManager::Clear()
{
	for (auto& stage : stages)
	{
		std::vector<std::shared_ptr<Stage>>::iterator it = std::find(stages.begin(), stages.end(), stage);
		if (it != stages.end()) stages.erase(it);
	}
}

// レイキャスト
bool StageManager::RayCast(const Vec3& start,
	const Vec3& end,
	HitResult& hit)
{
	bool result = false;
	hit.distance = FLT_MAX;
	for (auto& stage : stages)
	{
		HitResult tmp = {};
		if (stage->RayCast(start, end, tmp))
		{
			if (hit.distance > tmp.distance)
			{
				hit = tmp;
				result = true;
			}
		}
	}

	return result;
}
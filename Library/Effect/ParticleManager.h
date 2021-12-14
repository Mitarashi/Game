#pragma once

#include "../Library/Effect/Particle.h"

class ParticleManager
{
private:
	// コンストラクタ
	ParticleManager() = default;

	// デストラクタ
	~ParticleManager() = default;

public:
	// コンストラクタ
	ParticleManager(const ParticleManager&) = delete;
	ParticleManager(ParticleManager&&) = delete;

	// オペレーター
	ParticleManager& operator= (const ParticleManager&) = delete;
	ParticleManager& operator= (ParticleManager&&) = delete;

	// インスタンス作成
	static void Create();

	// インスタンス削除
	static void Destroy();

	// インスタンス取得
	static ParticleManager& Instance() { return *instance; }

	// 更新処理
	void Update();

	// 描画処理
	void Render(const RenderContext& renderContext);

	// 要素全削除
	void Clear();

	// パーティクル登録
	//void Register(std::unique_ptr<Particle> particle);

private:
	inline static ParticleManager* instance = nullptr;

	//std::vector<std::unique_ptr<Particle>> particles = {};
};
#pragma once

#include "../Library/Math.h"
#include "../Library/Camera/Camera.h"

// カメラマネージャー
class CameraManager
{
private:
	// コンストラクタ
	CameraManager();

	// デストラクタ
	~CameraManager() = default;

public:
	// コンストラクタ
	CameraManager(const CameraManager&) = delete;
	CameraManager(CameraManager&&) = delete;

	// オペレーター
	CameraManager& operator= (const CameraManager&) = delete;
	CameraManager& operator= (CameraManager&&) = delete;

	// インスタンス作成
	static void Create();

	// インスタンス削除
	static void Destroy();

	// インスタンス取得
	static CameraManager& Instance() { return *instance; }

	// 初期化
	void Initialize(
		const Vec3& eye		= Vec3(0, 10, -10),
		const Vec3& focus	= Vec3(0, 0, 0),
		const Vec3& up		= Vec3(0, 1, 0));

	// 更新処理
	void Update();

	// ターゲット設定
	void SetTarget(const Vec3& target_) { target = target_; }

	// ゲッター
	const std::unique_ptr<Camera>& GetMainCamera() const { return camera; }

private:
	// 定数
	static constexpr float MAX_ANGLE_X = DirectX::XMConvertToRadians( 45);
	static constexpr float MIN_ANGLE_X	= DirectX::XMConvertToRadians(-45);
	static constexpr float ROLL_SPEED	= DirectX::XMConvertToRadians(90);

private:
	inline static CameraManager*	instance	= nullptr;

	// 変数
	std::unique_ptr<Camera>			camera		= nullptr;

	Vec3							target		= {};
	Vec3							angle		= {};
	float							rollSpeed	= Math::ToRadian(90);
	float							range		= 10.0f;
};
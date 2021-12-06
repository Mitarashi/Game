#include "../Math.h"
#include "../DxSystem.h"
#include "../Input/InputManager.h"

#include "CameraManager.h"

// コンストラクタ
CameraManager::CameraManager() : camera(std::make_unique<Camera>())
{
}

// インスタンス作成
void CameraManager::Create()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, L"already instantiated\n(既にインスタンス化されています)");
	instance = new CameraManager;
}

// インスタンス削除
void CameraManager::Destroy()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// カメラの初期化
void CameraManager::Initialize(const Vec3& eye, const Vec3& focus, const Vec3& up)
{
	DxSystem& dxSystem = DxSystem::Instance();

	// スクリーンサイズ取得
	const float screenWidth = dxSystem.GetScreenWidth();
	const float screenheight = dxSystem.GetScreenHeight();

	// カメラを指定方向に向ける
	camera->SetLookAt(eye, focus, up);

	// 透視投影設定
	const float fovY = Math::ToRadian(45);
	const float aspect = screenWidth / screenheight;
	const float nearZ = 0.1f;
	const float farZ = 1000.0f;
	camera->SetPerspectiveFov(fovY, aspect, nearZ, farZ);
}

// カメラの更新
void CameraManager::Update()
{
	// スティック入力取得
	GamePad& gamePad = InputManager::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();

	// カメラの回転速度
	float speed = rollSpeed * DxSystem::Instance().GetDeltaTime();

	// スティックの入力に合わせてX軸とY軸を回転
	angle.x += ay * speed;
	angle.y += ax * speed;

	// X軸のカメラ回転を制限
	if (angle.x < MIN_ANGLE_X) angle.x = MIN_ANGLE_X;
	if (angle.x > MAX_ANGLE_X) angle.x = MAX_ANGLE_X;

	// Y軸の回転値を-3.14~3.14に収まるようにする
	angle.y = Math::AngleNormalize(angle.y);

	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// 回転行列から前方ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	Vec3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	Vec3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	// カメラに視点と注視点を設定
	camera->SetLookAt(eye, target, Vec3(0, 1, 0));
}
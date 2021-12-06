#include "../Input/InputManager.h"

#include "Camera.h"

// 指定方向を向く
void Camera::SetLookAt(
	const Vec3& eye_,
	const Vec3& focus_,
	const Vec3& up_)
{
	// 視点、注視点、上方向からビュー行列を作成
	DirectX::XMVECTOR Eye = eye_.GetXMVector();
	DirectX::XMVECTOR Focus = focus_.GetXMVector();
	DirectX::XMVECTOR Up = up_.GetXMVector();
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	// ビューを逆行列化し、ワールド行列に戻す
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world = {};
	DirectX::XMStoreFloat4x4(&world, World);

	// カメラの方向を取り出す
	this->right = { world._11, world._12, world._13 };
	this->up = { world._21, world._22, world._23 };
	this->front = { world._31, world._32, world._33 };

	// 視点、注視点を保存
	eye = eye_;
	focus = focus_;
}

// 透視投影設定
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	// 画角、画面比率、クリップ距離からプロジェクション行列作成
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
}

// 平行投影設定
void Camera::SetOrthographic(float width, float height, float nearZ, float farZ)
{
	DirectX::XMMATRIX Projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
}
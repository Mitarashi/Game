#include "../Library/Math.h"

#include "Obj3d.h"

// 行列更新処理
void Obj3d::UpdateTransform()
{
	// スケール行列を作成
	DirectX::XMMATRIX Scale = Math::MatrixScaling(scale);

	// 回転行列を作成
	DirectX::XMMATRIX Rotate = Math::MatrixRotation(angle);

	// 位置行列を作成
	DirectX::XMMATRIX Transform = Math::MatrixTranslation(position);

	// ワールド行列を作成
	DirectX::XMMATRIX World = Scale * Rotate * Transform;

	// ワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, World);
}
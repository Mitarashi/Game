#include "../Library/Math.h"

#include "Obj3d.h"

// �s��X�V����
void Obj3d::UpdateTransform()
{
	// �X�P�[���s����쐬
	DirectX::XMMATRIX Scale = Math::MatrixScaling(scale);

	// ��]�s����쐬
	DirectX::XMMATRIX Rotate = Math::MatrixRotation(angle);

	// �ʒu�s����쐬
	DirectX::XMMATRIX Transform = Math::MatrixTranslation(position);

	// ���[���h�s����쐬
	DirectX::XMMATRIX World = Scale * Rotate * Transform;

	// ���[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, World);
}
#pragma once

#include "../Library/Utility/Vector.h"

// �����_�[�R���e�L�X�g
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	Vec4					lightDirection;
};

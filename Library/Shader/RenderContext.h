#pragma once

#include "../Library/Utility/Vector.h"

// レンダーコンテキスト
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	Vec4					lightDirection;
};

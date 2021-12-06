#pragma once

#include <crtdbg.h>
#include <windows.h>

#include "ResourceManager.h"

#include "Math.h"

#undef TRUE
#undef FALSE

#if (_MSVC_LANG > 201704L) // /std:c++latest = 201705L /std:c++17 = 201703L
#define CPLUSPLUS_VER_20 (1)
#else
#define CPLUSPLUS_VER_20 (0)
#endif

#if defined( DEBUG ) || defined( _DEBUG )
#define _ASSERT_EXPR_A(expr, msg) \
	(void)((!!(expr)) || \
	(1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, nullptr, "%s", msg)) || \
	(_CrtDbgBreak(), 0))
#else
#define  _ASSERT_EXPR_A(expr, expr_str) ((void)0)
#endif

inline LPWSTR HRTrace(HRESULT hr)
{
	LPWSTR msg;
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_ALLOCATE_BUFFER,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&msg), 0, nullptr);
	return msg;
}

// TODO Ç¢ÇÁÇ»Ç¢Ç©Ç‡
// enum class Ç©ÇÁílÇéÊÇËèoÇ∑
template<class Type>
inline static int GetNum(const Type state)
{
	return static_cast<u_int>(state);
}

namespace Utility
{
	inline float IntersectionInsideLine(Vec3 p, Vec3 v1, Vec3 v2, Vec3 normal)
	{
		DirectX::XMVECTOR V = DirectX::XMVectorSubtract(v1.GetXMVector(), p.GetXMVector());
		DirectX::XMVECTOR Cross = DirectX::XMVector3Cross(V, v2.GetXMVector());
		Vec3 cross = Vec3(Cross);
		return Math::Vector3Dot(cross, normal);
	}
};
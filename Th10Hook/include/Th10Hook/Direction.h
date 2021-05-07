#pragma once

#include "Th10Hook/Common.h"

#include <Base/TypeTraits.h>

namespace th
{
	// 移动方向
	enum class DIR : uint_t
	{
		HOLD,			// 不动
		UP,				// 上
		DOWN,			// 下
		LEFT,			// 左
		RIGHT,			// 右
		LEFTUP,			// 左上
		RIGHTUP,		// 右上
		LEFTDOWN,		// 左下
		RIGHTDOWN,		// 右下

		MAX_COUNT
	};

	constexpr DIR DIRS[to_underlying(DIR::MAX_COUNT)] =
	{
		DIR::HOLD,
		DIR::UP,
		DIR::DOWN,
		DIR::LEFT,
		DIR::RIGHT,
		DIR::LEFTUP,
		DIR::RIGHTUP,
		DIR::LEFTDOWN,
		DIR::RIGHTDOWN
	};
}

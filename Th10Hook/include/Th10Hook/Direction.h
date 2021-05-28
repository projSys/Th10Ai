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
		UP_LEFT,		// 左上
		UP_RIGHT,		// 右上
		DOWN_LEFT,		// 左下
		DOWN_RIGHT,		// 右下

		MAX_COUNT
	};

	constexpr DIR DIRS[to_underlying(DIR::MAX_COUNT)] =
	{
		DIR::HOLD,
		DIR::UP,
		DIR::DOWN,
		DIR::LEFT,
		DIR::RIGHT,
		DIR::UP_LEFT,
		DIR::UP_RIGHT,
		DIR::DOWN_LEFT,
		DIR::DOWN_RIGHT
	};
}

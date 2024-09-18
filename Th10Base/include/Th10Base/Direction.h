#pragma once

#include "Th10Base/Common.h"

#include "Base/Traits.h"

namespace th
{
	// 移动方向
	enum class Direction : uint_t
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

	constexpr Direction g_directions[to_underlying(Direction::MAX_COUNT)] =
	{
		Direction::HOLD,
		Direction::UP,
		Direction::DOWN,
		Direction::LEFT,
		Direction::RIGHT,
		Direction::UP_LEFT,
		Direction::UP_RIGHT,
		Direction::DOWN_LEFT,
		Direction::DOWN_RIGHT
	};
}

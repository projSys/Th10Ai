#pragma once

#include "Th10Ai/Common.h"

#include <array>

namespace th
{
	// 移动方向
	enum class DIR : uint_t
	{
		HOLD,			// 不动
		LEFT,			// 左
		RIGHT,			// 右
		UP,				// 上
		DOWN,			// 下
		LEFTUP,			// 左上
		LEFTDOWN,		// 左下
		RIGHTUP,		// 右上
		RIGHTDOWN,		// 右下

		MAX_COUNT
	};

	constexpr std::array<DIR, enum_cast(DIR::MAX_COUNT)> g_dirs =
	{ {
		DIR::HOLD,
		DIR::LEFT,
		DIR::RIGHT,
		DIR::UP,
		DIR::DOWN,
		DIR::LEFTUP,
		DIR::LEFTDOWN,
		DIR::RIGHTUP,
		DIR::RIGHTDOWN
	} };
}

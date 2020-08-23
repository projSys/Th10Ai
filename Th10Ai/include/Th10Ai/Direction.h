#pragma once

#include <Base/Enum.h>

namespace comm
{
	// 移动方向
	enum Direction
	{
		DIR_NONE = -1,

		DIR_HOLD,			// 不动
		DIR_LEFT,			// 左
		DIR_RIGHT,			// 右
		DIR_UP,				// 上
		DIR_DOWN,			// 下
		DIR_LEFTUP,			// 左上
		DIR_RIGHTUP,		// 右上
		DIR_LEFTDOWN,		// 左下
		DIR_RIGHTDOWN,		// 右下

		DIR_MAXCOUNT
	};

	const Direction DIRECTIONS[] =
	{
		DIR_HOLD,
		DIR_LEFT,
		DIR_RIGHT,
		DIR_UP,
		DIR_DOWN,
		DIR_LEFTUP,
		DIR_RIGHTUP,
		DIR_LEFTDOWN,
		DIR_RIGHTDOWN
	};

	template <>
	class EnumTraits<Direction>
	{
	public:
		static constexpr Direction ENUMERATORS[] =
		{
			DIR_HOLD,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_UP,
			DIR_DOWN,
			DIR_LEFTUP,
			DIR_RIGHTUP,
			DIR_LEFTDOWN,
			DIR_RIGHTDOWN
		};

		static constexpr Direction NONE = DIR_NONE;
	};

	using Dir = Enum<Direction>;
}

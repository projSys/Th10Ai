#pragma once

namespace th
{
	// 移动方向
	enum Direction
	{
		DIR_NONE = -1,
		DIR_HOLD,			// 不动
		DIR_UP,				// 上
		DIR_DOWN,			// 下
		DIR_LEFT,			// 左
		DIR_RIGHT,			// 右
		DIR_UPLEFT,			// 左上
		DIR_UPRIGHT,		// 右上
		DIR_DOWNLEFT,		// 左下
		DIR_DOWNRIGHT,		// 右下
		DIR_MAXCOUNT
	};
}

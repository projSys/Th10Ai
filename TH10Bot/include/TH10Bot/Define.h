#pragma once

#include "TH10Bot/Point2.h"
#include "TH10Bot/Rect2.h"

namespace th
{
	// 场景坐标
	//(-200,0)        (0,0)      (200,0)
	//   +-----------+-----------+ X
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   |           |           |
	//   +-----------+-----------+
	//(-200,480)     Y(0,480)    (200,480)

	const double SCENE_WIDTH = 400.0;
	const double SCENE_HEIGHT = 480.0;

	// 初始位置
	const Point2d INIT_POS = { 0.0, 400.0 };
	const Rect2d INIT_RECT = { 0.0, 400.0, 4.0, 4.0 };



	// 方向
	const int DIR_CENTER = 0x00;
	const int DIR_UP = 0x01;
	const int DIR_DOWN = 0x02;
	const int DIR_LEFT = 0x04;
	const int DIR_RIGHT = 0x08;

	const int DIRECTION[] =
	{
		DIR_CENTER,				// 中
		DIR_UP | DIR_LEFT,		// 左上
		DIR_UP,					// 上
		DIR_UP | DIR_RIGHT,		// 右上
		DIR_LEFT,				// 左
		DIR_RIGHT,				// 右
		DIR_DOWN | DIR_LEFT,	// 左下
		DIR_DOWN,				// 下
		DIR_DOWN | DIR_RIGHT	// 右下
	};

	const int DIR_LEN = sizeof(DIRECTION) / sizeof(DIRECTION[0]);

	const double SQRT_PF = std::sqrt(0.5);

	// 各方向的移动系数
	const Point2d DIR_FACTOR[] =
	{
		{ 0.0, 0.0 },			// 中
		{ -SQRT_PF, -SQRT_PF },	// 左上
		{ 0.0, -1.0 },			// 上
		{ SQRT_PF, -SQRT_PF },	// 右上
		{ -1.0, 0.0 },			// 左
		{ 1.0, 0.0 },			// 右
		{ -SQRT_PF, SQRT_PF },	// 左下
		{ 0.0, 1.0 },			// 下
		{ SQRT_PF, SQRT_PF }	// 右下
	};

	const int DIR_FACTOR_LEN = sizeof(DIR_FACTOR) / sizeof(DIR_FACTOR[0]);

	//static_assert(DIR_LEN == DIR_FACTOR_LEN);

	// 移动速度
	const double MOVE_SPEED[] =
	{
		4.5,
		2.0
	};

	const int MOVE_SPEED_LEN = sizeof(MOVE_SPEED) / sizeof(MOVE_SPEED[0]);



	// 场景的最大距离，作为评分分母 624.81
	const double SCENE_MAXDIST = std::sqrt(SCENE_WIDTH * SCENE_WIDTH + SCENE_HEIGHT * SCENE_HEIGHT);
}

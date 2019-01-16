#pragma once

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

	const float_t SCENE_WIDTH = 400;
	const float_t SCENE_HEIGHT = 480;

	const Pointf SCENE_OFFSET = { 24, 16 };

	const Pointf POS_OFFSET = { 200, 0 };

	// 初始位置
	const Pointf INIT_POS = { 0, 400 };



	// 方向
	const int_t DIR_CENTER = 0x00;
	const int_t DIR_UP = 0x01;
	const int_t DIR_DOWN = 0x02;
	const int_t DIR_LEFT = 0x04;
	const int_t DIR_RIGHT = 0x08;

	const int_t DIRECTION[] =
	{
		DIR_UP | DIR_LEFT,		// 5 左上
		DIR_UP,					// 1 上
		DIR_UP | DIR_RIGHT,		// 9 右上
		DIR_LEFT,				// 4 左
		DIR_CENTER,				// 0 中
		DIR_RIGHT,				// 8 右
		DIR_DOWN | DIR_LEFT,	// 6 左下
		DIR_DOWN,				// 2 下
		DIR_DOWN | DIR_RIGHT	// 10 右下
	};

	const int_t DIR_LEN = sizeof(DIRECTION) / sizeof(DIRECTION[0]);



	const float_t SQRT_PF = std::sqrt(0.5f);

	// 各方向的移动系数
	const Pointf DIR_FACTOR[] =
	{
		{ -SQRT_PF, -SQRT_PF },	// 左上
		{ 0.0, -1.0 },			// 上
		{ SQRT_PF, -SQRT_PF },	// 右上
		{ -1.0, 0.0 },			// 左
		{ 0.0, 0.0 },			// 中
		{ 1.0, 0.0 },			// 右
		{ -SQRT_PF, SQRT_PF },	// 左下
		{ 0.0, 1.0 },			// 下
		{ SQRT_PF, SQRT_PF }	// 右下
	};

	const int_t DIR_FACTOR_LEN = sizeof(DIR_FACTOR) / sizeof(DIR_FACTOR[0]);

	//static_assert(DIR_LEN == DIR_FACTOR_LEN);

	// 移动速度
	const float_t MOVE_SPEED[] =
	{
		4.5,
		2.0
	};

	const int_t MOVE_SPEED_LEN = sizeof(MOVE_SPEED) / sizeof(MOVE_SPEED[0]);



	// 场景的最大距离，作为评分分母 624.81
	const float_t SCENE_MAXDIST = std::sqrt(SCENE_WIDTH * SCENE_WIDTH + SCENE_HEIGHT * SCENE_HEIGHT);



	// 裁剪角度和距离
	const float_t CLIP_ANGLE = 45;
	const float_t CLIP_DISTANCE = 100;
}

#pragma once

namespace th
{
	const Pointf SCENE_POS = { 24.0f, 16.0f };

	const Sizef SCENE_SIZE = { 384.0f, 448.0f };

	const Pointf POS_OFFSET = { 200.0f, 0.0f };

	const Pointf INIT_POS = { 0.0f, 400.0f };

	// ����
	enum Direction
	{
		DIR_NONE = -1,
		DIR_HOLD_SLOW,
		DIR_HOLD,				// ����
		DIR_UP,					// ��
		DIR_DOWN,				// ��
		DIR_LEFT,				// ��
		DIR_RIGHT,				// ��
		DIR_UPLEFT,				// ����
		DIR_UPRIGHT,			// ����
		DIR_DOWNLEFT,			// ����
		DIR_DOWNRIGHT,			// ����
		DIR_UP_SLOW,
		DIR_DOWN_SLOW,
		DIR_LEFT_SLOW,
		DIR_RIGHT_SLOW,
		DIR_UPLEFT_SLOW,
		DIR_UPRIGHT_SLOW,
		DIR_DOWNLEFT_SLOW,
		DIR_DOWNRIGHT_SLOW,
		DIR_MAXCOUNT
	};

	const Direction DIRECTIONS[DIR_MAXCOUNT] =
	{
		DIR_HOLD_SLOW,
		DIR_HOLD,
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
		DIR_UPLEFT,
		DIR_UPRIGHT,
		DIR_DOWNLEFT,
		DIR_DOWNRIGHT,
		DIR_UP_SLOW,
		DIR_DOWN_SLOW,
		DIR_LEFT_SLOW,
		DIR_RIGHT_SLOW,
		DIR_UPLEFT_SLOW,
		DIR_UPRIGHT_SLOW,
		DIR_DOWNLEFT_SLOW,
		DIR_DOWNRIGHT_SLOW,
	};

	const Direction PD[DIR_UP_SLOW][DIR_UP_SLOW] =
	{
		{	// DIR_HOLD_SLOW
			DIR_HOLD_SLOW,
			DIR_HOLD,
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_UPLEFT,
			DIR_UPRIGHT,
			DIR_DOWNLEFT,
			DIR_DOWNRIGHT
		},
		{	// DIR_HOLD
			DIR_HOLD_SLOW,
			DIR_HOLD,
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_UPLEFT,
			DIR_UPRIGHT,
			DIR_DOWNLEFT,
			DIR_DOWNRIGHT
		},
		{	// DIR_UP
			DIR_LEFT,
			DIR_RIGHT,
			DIR_UPLEFT,
			DIR_UPRIGHT,
			DIR_DOWNLEFT,
			DIR_DOWNRIGHT,
			DIR_UP,
			DIR_DOWN,
			DIR_HOLD_SLOW,
			DIR_HOLD
		},
		{	// DIR_DOWN
			DIR_LEFT,
			DIR_RIGHT,
			DIR_DOWNLEFT,
			DIR_DOWNRIGHT,
			DIR_UPLEFT,
			DIR_UPRIGHT,
			DIR_DOWN,
			DIR_UP,
			DIR_HOLD_SLOW,
			DIR_HOLD
		},
		{	// DIR_LEFT
			DIR_UP,
			DIR_DOWN,
			DIR_UPLEFT,
			DIR_DOWNLEFT,
			DIR_UPRIGHT,
			DIR_DOWNRIGHT,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_HOLD_SLOW,
			DIR_HOLD
		},
		{	// DIR_RIGHT
			DIR_UP,
			DIR_DOWN,
			DIR_UPRIGHT,
			DIR_DOWNRIGHT,
			DIR_UPLEFT,
			DIR_DOWNLEFT,
			DIR_RIGHT,
			DIR_LEFT,
			DIR_HOLD_SLOW,
			DIR_HOLD
		},
		{	// DIR_UPLEFT
			DIR_HOLD_SLOW,
			DIR_HOLD,
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_UPLEFT,
			DIR_UPRIGHT,
			DIR_DOWNLEFT,
			DIR_DOWNRIGHT
		},
		{	// DIR_UPRIGHT
			DIR_HOLD_SLOW,
			DIR_HOLD,
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_UPLEFT,
			DIR_UPRIGHT,
			DIR_DOWNLEFT,
			DIR_DOWNRIGHT
		},
		{	// DIR_DOWNLEFT
			DIR_HOLD_SLOW,
			DIR_HOLD,
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_UPLEFT,
			DIR_UPRIGHT,
			DIR_DOWNLEFT,
			DIR_DOWNRIGHT
		},
		{	// DIR_DOWNRIGHT
			DIR_HOLD_SLOW,
			DIR_HOLD,
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT,
			DIR_UPLEFT,
			DIR_UPRIGHT,
			DIR_DOWNLEFT,
			DIR_DOWNRIGHT
		}
	};

	const float_t PS[DIR_UP_SLOW] =
	{
		1.5f,
		1.5f,
		1.4f,
		1.4f,
		1.3f,
		1.3f,
		1.0f,
		0.8f,
		0.6f,
		0.6f
	};

	// б������ƶ�ϵ�� x^2 + x^2 = 1^2
	const float_t SQRT_PF = std::sqrt(0.5f);

	// ��������ƶ�ϵ��
	const Pointf DIR_FACTORS[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },
		{ 0.0f, 0.0f },				// ����
		{ 0.0f, -1.0f },			// ��
		{ 0.0f, 1.0f },				// ��
		{ -1.0f, 0.0f },			// ��
		{ 1.0f, 0.0f },				// ��
		{ -SQRT_PF, -SQRT_PF },		// ����
		{ SQRT_PF, -SQRT_PF },		// ����
		{ -SQRT_PF, SQRT_PF },		// ����
		{ SQRT_PF, SQRT_PF },		// ����
		{ 0.0f, -1.0f },
		{ 0.0f, 1.0f },
		{ -1.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ -SQRT_PF, -SQRT_PF },
		{ SQRT_PF, -SQRT_PF },
		{ -SQRT_PF, SQRT_PF },
		{ SQRT_PF, SQRT_PF }
	};

	// �ƶ��ٶ�
	const float_t MOVE_SPEED = 4.5f;
	const float_t MOVE_SPEED_SLOW = 2.0f;
}

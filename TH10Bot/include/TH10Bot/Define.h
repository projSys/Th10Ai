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
		DIR_HOLD,				// ����
		DIR_UP,					// ��
		DIR_DOWN,				// ��
		DIR_LEFT,				// ��
		DIR_RIGHT,				// ��
		DIR_UPLEFT,				// ����
		DIR_UPRIGHT,			// ����
		DIR_DOWNLEFT,			// ����
		DIR_DOWNRIGHT,			// ����
		DIR_HOLD_SLOW,
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
		DIR_HOLD,
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
		DIR_UPLEFT,
		DIR_UPRIGHT,
		DIR_DOWNLEFT,
		DIR_DOWNRIGHT,
		DIR_HOLD_SLOW,
		DIR_UP_SLOW,
		DIR_DOWN_SLOW,
		DIR_LEFT_SLOW,
		DIR_RIGHT_SLOW,
		DIR_UPLEFT_SLOW,
		DIR_UPRIGHT_SLOW,
		DIR_DOWNLEFT_SLOW,
		DIR_DOWNRIGHT_SLOW,
	};

	const Direction PD[DIR_HOLD_SLOW][DIR_HOLD_SLOW] =
	{
		// DIR_HOLD
		{ DIR_HOLD,    DIR_HOLD,      DIR_HOLD,     DIR_HOLD,      DIR_HOLD,      DIR_HOLD,  DIR_HOLD,     DIR_HOLD,      DIR_HOLD },
		// DIR_UP
		{ DIR_LEFT,    DIR_RIGHT,     DIR_UPLEFT,   DIR_UPRIGHT,   DIR_UP,        DIR_HOLD,  DIR_DOWNLEFT, DIR_DOWNRIGHT, DIR_DOWN },
		// DIR_DOWN
		{ DIR_LEFT,    DIR_RIGHT,     DIR_DOWNLEFT, DIR_DOWNRIGHT, DIR_DOWN,      DIR_HOLD,  DIR_UPLEFT,   DIR_UPRIGHT,   DIR_UP },
		// DIR_LEFT
		{ DIR_UP,      DIR_DOWN,      DIR_UPLEFT,   DIR_DOWNLEFT,  DIR_LEFT,      DIR_HOLD,  DIR_UPRIGHT,  DIR_DOWNRIGHT, DIR_RIGHT },
		// DIR_RIGHT
		{ DIR_UP,      DIR_DOWN,      DIR_UPRIGHT,  DIR_DOWNRIGHT, DIR_RIGHT,     DIR_HOLD,  DIR_UPLEFT,   DIR_DOWNLEFT,  DIR_LEFT },
		// DIR_UPLEFT
		{ DIR_UPRIGHT, DIR_DOWNLEFT,  DIR_UP,       DIR_LEFT,      DIR_UPLEFT,    DIR_HOLD,  DIR_DOWN,     DIR_RIGHT,     DIR_DOWNRIGHT },
		// DIR_UPRIGHT
		{ DIR_UPLEFT,  DIR_DOWNRIGHT, DIR_UP,       DIR_RIGHT,     DIR_UPRIGHT,   DIR_HOLD,  DIR_DOWN,     DIR_LEFT,      DIR_DOWNLEFT },
		// DIR_DOWNLEFT
		{ DIR_UPLEFT,  DIR_DOWNRIGHT, DIR_DOWN,     DIR_LEFT,      DIR_DOWNLEFT,  DIR_HOLD,  DIR_UP,       DIR_RIGHT,     DIR_UPRIGHT },
		// DIR_DOWNRIGHT
		{ DIR_UPRIGHT, DIR_DOWNLEFT,  DIR_DOWN,     DIR_RIGHT,     DIR_DOWNRIGHT, DIR_HOLD,  DIR_UP,       DIR_LEFT,      DIR_UPLEFT }
	};

	const float_t PS[DIR_HOLD_SLOW] =
	{
		120.0f,
		120.0f,
		110.0f,
		110.0f,
		100.0f,
		90.0f,
		80.0f,
		80.0f,
		70.0f
	};

	// ��������ƶ��ٶ�
	const Pointf MOVE_SPEED[DIR_MAXCOUNT] =
	{
		{ 0.0f, 0.0f },			// ����
		{ 0.0f, -4.5f },		// ��
		{ 0.0f, 4.5f },			// ��
		{ -4.5f, 0.0f },		// ��
		{ 4.5f, 0.0f },			// ��
		{ -3.18f, -3.18f },		// ����
		{ 3.18f, -3.18f },		// ����
		{ -3.18f, 3.18f },		// ����
		{ 3.18f, 3.18f },		// ����
		{ 0.0f, 0.0f },
		{ 0.0f, -2.0f },
		{ 0.0f, 2.0f },
		{ -2.0f, 0.0f },
		{ 2.0f, 0.0f },
		{ -1.41f, -1.41f },
		{ 1.41f, -1.41f },
		{ -1.41f, 1.41f },
		{ 1.41f, 1.41f }
	};
}

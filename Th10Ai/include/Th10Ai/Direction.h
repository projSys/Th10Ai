#pragma once

namespace th
{
	// �ƶ�����
	enum Direction
	{
		DIR_NONE = -1,

		DIR_HOLD,			// ����
		DIR_LEFT,			// ��
		DIR_RIGHT,			// ��
		DIR_UP,				// ��
		DIR_DOWN,			// ��
		DIR_LEFTUP,			// ����
		DIR_RIGHTUP,		// ����
		DIR_LEFTDOWN,		// ����
		DIR_RIGHTDOWN,		// ����

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
}

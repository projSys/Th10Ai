#pragma once

#include <Base/Enum.h>

namespace comm
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

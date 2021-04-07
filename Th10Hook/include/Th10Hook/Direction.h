#pragma once

#include "Th10Hook/Common.h"

namespace th
{
	// �ƶ�����
	enum class DIR : uint_t
	{
		HOLD,			// ����
		UP,				// ��
		DOWN,			// ��
		LEFT,			// ��
		RIGHT,			// ��
		LEFTUP,			// ����
		RIGHTUP,		// ����
		LEFTDOWN,		// ����
		RIGHTDOWN,		// ����

		MAX_COUNT
	};

	constexpr DIR DIRS[enum_cast(DIR::MAX_COUNT)] =
	{
		DIR::HOLD,
		DIR::UP,
		DIR::DOWN,
		DIR::LEFT,
		DIR::RIGHT,
		DIR::LEFTUP,
		DIR::RIGHTUP,
		DIR::LEFTDOWN,
		DIR::RIGHTDOWN
	};
}

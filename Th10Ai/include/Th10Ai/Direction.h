#pragma once

#include "Th10Ai/Common.h"

#include <Base/Enum.h>

namespace th
{
	// �ƶ�����
	enum class DIR : uint_t
	{
		HOLD,			// ����
		LEFT,			// ��
		RIGHT,			// ��
		UP,				// ��
		DOWN,			// ��
		LEFTUP,			// ����
		RIGHTUP,		// ����
		LEFTDOWN,		// ����
		RIGHTDOWN		// ����
	};

	constexpr DIR DIR_ENTRIES[] =
	{
		DIR::HOLD,
		DIR::LEFT,
		DIR::RIGHT,
		DIR::UP,
		DIR::DOWN,
		DIR::LEFTUP,
		DIR::RIGHTUP,
		DIR::LEFTDOWN,
		DIR::RIGHTDOWN
	};

	constexpr uint_t DIR_MAXCOUNT = sizeof(DIR_ENTRIES) / sizeof(DIR_ENTRIES[0]);
}

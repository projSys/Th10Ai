#pragma once

#include "Th10Ai/Common.h"

#include <array>

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
		LEFTDOWN,		// ����
		RIGHTUP,		// ����
		RIGHTDOWN,		// ����

		MAX_COUNT
	};

	constexpr std::array<DIR, enum_cast(DIR::MAX_COUNT)> g_dirs =
	{ {
		DIR::HOLD,
		DIR::LEFT,
		DIR::RIGHT,
		DIR::UP,
		DIR::DOWN,
		DIR::LEFTUP,
		DIR::LEFTDOWN,
		DIR::RIGHTUP,
		DIR::RIGHTDOWN
	} };
}

#pragma once

#include "Th10Ai/Common.h"

#include <array>
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

	constexpr std::array<DIR, 9> g_dirs =
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
}

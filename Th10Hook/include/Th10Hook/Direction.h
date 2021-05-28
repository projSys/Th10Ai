#pragma once

#include "Th10Hook/Common.h"

#include <Base/TypeTraits.h>

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
		UP_LEFT,		// ����
		UP_RIGHT,		// ����
		DOWN_LEFT,		// ����
		DOWN_RIGHT,		// ����

		MAX_COUNT
	};

	constexpr DIR DIRS[to_underlying(DIR::MAX_COUNT)] =
	{
		DIR::HOLD,
		DIR::UP,
		DIR::DOWN,
		DIR::LEFT,
		DIR::RIGHT,
		DIR::UP_LEFT,
		DIR::UP_RIGHT,
		DIR::DOWN_LEFT,
		DIR::DOWN_RIGHT
	};
}

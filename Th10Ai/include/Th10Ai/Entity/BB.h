#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/Vector2.h"
#include "Th10Ai/Entity/Projection.h"

namespace th
{
	// ��Χ��
	class BB
	{
	public:
		// ͶӰ����2ά����1ά
		Projection project(const vec2& axis) const;
		// ƽ��
		void translate(const vec2& offset);

	public:
		vec2 m_topLeft;
		vec2 m_topRight;
		vec2 m_bottomLeft;
		vec2 m_bottomRight;
	};
}

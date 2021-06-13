#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/Vector2.h"
#include "Th10Ai/Entity/Projection.h"

namespace th
{
	// 包围盒
	class BB
	{
	public:
		// 投影，把2维降成1维
		Projection project(const vec2& axis) const;
		// 平移
		void translate(const vec2& offset);

	public:
		vec2 m_topLeft;
		vec2 m_topRight;
		vec2 m_bottomLeft;
		vec2 m_bottomRight;
	};
}

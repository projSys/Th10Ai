#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/BB.h"

namespace th
{
	class AABB;

	// 定向包围盒
	class OBB :
		public BB
	{
	public:
		bool collide(const AABB& other) const;

	protected:
		vec2 m_axisX;	// 投影轴，单位向量
		vec2 m_axisY;
	};
}

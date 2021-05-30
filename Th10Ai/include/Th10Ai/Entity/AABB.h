#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/BB.h"

namespace th
{
	// ������Χ��
	class AABB :
		public BB
	{
	public:
		Projection horizontalProject() const;
		Projection verticalProject() const;
		bool collide(const AABB& other) const;
	};
}

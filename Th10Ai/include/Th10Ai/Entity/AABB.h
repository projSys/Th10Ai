#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/BB.h"

namespace th
{
	// Öá¶ÔÆë°üÎ§ºÐ
	class AABB :
		public BB
	{
	public:
		Projection horizontalProject() const;
		Projection verticalProject() const;
		bool collide(const AABB& other) const;
	};
}

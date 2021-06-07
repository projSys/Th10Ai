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
		Projection projectX() const;
		Projection projectY() const;
		bool collide(const AABB& other) const;
	};
}

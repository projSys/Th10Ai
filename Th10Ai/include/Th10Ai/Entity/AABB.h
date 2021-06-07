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
		Projection projectX() const;
		Projection projectY() const;
		bool collide(const AABB& other) const;
	};
}

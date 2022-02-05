#include "Th10Ai/Entity/AABB.h"

namespace th
{
	Projection AABB::projectX() const
	{
		return Projection(topLeft.x, bottomRight.x);
	}

	Projection AABB::projectY() const
	{
		return Projection(topLeft.y, bottomRight.y);
	}

	bool AABB::collide(const AABB& other) const
	{
		// SAT分离轴定理
		if (!projectX().overlap(other.projectX()))
			return false;
		if (!projectY().overlap(other.projectY()))
			return false;
		return true;
	}
}

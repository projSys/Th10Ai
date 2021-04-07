#include "Th10Hook/Math/AABB.h"

#include "Th10Hook/Entity.h"

namespace th
{
	AABB::AABB(const Entity& entity)
	{
		leftTop = entity.getLeftTop();
		rightTop = entity.getRightTop();
		leftBottom = entity.getLeftBottom();
		rightBottom = entity.getRightBottom();
		axisX = vec2(1, 0);
		axisY = vec2(0, 1);
	}

	bool Overlap(float_t min1, float_t max1, float_t min2, float_t max2)
	{
		//return !(max1 < min2 || max2 < min1);
		return max1 > min2 && max2 > min1;
	}

	bool AABB::collide(const AABB& other) const
	{
		if (!Overlap(leftTop.x, rightBottom.x, other.leftTop.x, other.rightBottom.x))
			return false;
		if (!Overlap(leftTop.y, rightBottom.y, other.leftTop.y, other.rightBottom.y))
			return false;
		return true;
	}
}

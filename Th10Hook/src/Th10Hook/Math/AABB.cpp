#include "Th10Hook/Math/AABB.h"

#include "Th10Hook/Entity.h"
#include "Th10Hook/Math/Projection.h"

namespace th
{
	AABB::AABB(const Entity& entity)
	{
		update(entity);
	}

	void AABB::update(const Entity& entity)
	{
		leftTop = entity.getLeftTop();
		rightTop = entity.getRightTop();
		leftBottom = entity.getLeftBottom();
		rightBottom = entity.getRightBottom();
		axisX = vec2(_F(1.0), _F(0.0));
		axisY = vec2(_F(0.0), _F(1.0));
	}

	bool AABB::collide(const AABB& other) const
	{
		if (!Projection(leftTop.x, rightBottom.x).overlap(
			Projection(other.leftTop.x, other.rightBottom.x)))
			return false;
		if (!Projection(leftTop.y, rightBottom.y).overlap(
			Projection(other.leftTop.y, other.rightBottom.y)))
			return false;
		return true;
	}
}

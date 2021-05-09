#include "Th10Hook/Math/AABB.h"

#include "Th10Hook/Entity.h"
#include "Th10Hook/Math/CollisionDetection.h"

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
		if (!CollisionDetection::Overlap(
			leftTop.x, rightBottom.x, other.leftTop.x, other.rightBottom.x))
			return false;
		if (!CollisionDetection::Overlap(
			leftTop.y, rightBottom.y, other.leftTop.y, other.rightBottom.y))
			return false;
		return true;
	}
}

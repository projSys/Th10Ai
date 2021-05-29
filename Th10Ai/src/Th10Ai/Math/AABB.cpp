#include "Th10Ai/Math/AABB.h"

#include "Th10Ai/Entity.h"
#include "Th10Ai/Math/Projection.h"

namespace th
{
	AABB::AABB(const Entity& entity)
	{
		update(entity);
	}

	void AABB::update(const Entity& entity)
	{
		topLeft = entity.getTopLeft();
		topRight = entity.getTopRight();
		bottomLeft = entity.getBottomLeft();
		bottomRight = entity.getBottomRight();
		axisX = vec2(_F(1.0), _F(0.0));
		axisY = vec2(_F(0.0), _F(1.0));
	}

	bool AABB::collide(const AABB& other) const
	{
		if (!Projection(topLeft.x, bottomRight.x).overlap(
			Projection(other.topLeft.x, other.bottomRight.x)))
			return false;
		if (!Projection(topLeft.y, bottomRight.y).overlap(
			Projection(other.topLeft.y, other.bottomRight.y)))
			return false;
		return true;
	}
}

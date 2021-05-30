#include "Th10Ai/Entity/AABB.h"

#include "Th10Ai/Entity/Projection.h"

namespace th
{
	Projection AABB::horizontalProject() const
	{
		return Projection(m_topLeft.x, m_bottomRight.x);
	}

	Projection AABB::verticalProject() const
	{
		return Projection(m_topLeft.y, m_bottomRight.y);
	}

	bool AABB::collide(const AABB& other) const
	{
		// SAT∑÷¿Î÷·∂®¿Ì
		if (!horizontalProject().overlap(other.horizontalProject()))
			return false;
		if (!verticalProject().overlap(other.verticalProject()))
			return false;
		return true;
	}
}

#include "Th10Ai/Entity/OBB.h"

#include "Th10Ai/Entity/AABB.h"

namespace th
{
	bool OBB::collide(const AABB& other) const
	{
		// SAT∑÷¿Î÷·∂®¿Ì
		if (!project(AABB::PROJ_AXIS_X).overlap(other.project(AABB::PROJ_AXIS_X)))
			return false;
		if (!project(AABB::PROJ_AXIS_Y).overlap(other.project(AABB::PROJ_AXIS_Y)))
			return false;
		if (!project(m_projAxisX).overlap(other.project(m_projAxisX)))
			return false;
		if (!project(m_projAxisY).overlap(other.project(m_projAxisY)))
			return false;
		return true;
	}
}

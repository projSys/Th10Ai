#include "Th10Ai/Entity/OBB.h"

#include "Th10Ai/Entity/AABB.h"

namespace th
{
	bool OBB::collide(const AABB& other) const
	{
		// SAT分离轴定理
		if (!project(AABB::PROJ_AXIS_X).overlap(other.project(AABB::PROJ_AXIS_X)))
			return false;
		if (!project(AABB::PROJ_AXIS_Y).overlap(other.project(AABB::PROJ_AXIS_Y)))
			return false;
		if (!project(projAxisX).overlap(other.project(projAxisX)))
			return false;
		if (!project(projAxisY).overlap(other.project(projAxisY)))
			return false;
		return true;
	}
}

#include "Th10Base/OBB.h"

#include "Th10Base/AABB.h"

namespace th
{
	bool OBB::collide(const AABB& other) const
	{
		// SAT分离轴定理
		if (!projectByNormalized(AABB::PROJ_AXIS_X).overlap(other.projectByNormalized(AABB::PROJ_AXIS_X)))
			return false;
		if (!projectByNormalized(AABB::PROJ_AXIS_Y).overlap(other.projectByNormalized(AABB::PROJ_AXIS_Y)))
			return false;
		if (!projectByNormalized(projAxisX).overlap(other.projectByNormalized(projAxisX)))
			return false;
		if (!projectByNormalized(projAxisY).overlap(other.projectByNormalized(projAxisY)))
			return false;
		return true;
	}
}

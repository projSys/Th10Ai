#include "Th10Base/OBB.h"

#include "Th10Base/AABB.h"

namespace th
{
	bool OBB::collide(const AABB& other) const
	{
		// SAT分离轴定理
		if (!projectWithNormalized(AABB::PROJ_AXIS_X).overlap(other.projectWithNormalized(AABB::PROJ_AXIS_X)))
			return false;
		if (!projectWithNormalized(AABB::PROJ_AXIS_Y).overlap(other.projectWithNormalized(AABB::PROJ_AXIS_Y)))
			return false;
		if (!projectWithNormalized(projAxisX).overlap(other.projectWithNormalized(projAxisX)))
			return false;
		if (!projectWithNormalized(projAxisY).overlap(other.projectWithNormalized(projAxisY)))
			return false;
		return true;
	}
}

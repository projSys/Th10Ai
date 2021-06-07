#include "Th10Ai/Entity/OBB.h"

#include "Th10Ai/Entity/AABB.h"

namespace th
{
	bool OBB::collide(const AABB& other) const
	{
		// AABB的投影轴
		vec2 axisX(_F(1.0), _F(0.0));
		vec2 axisY(_F(0.0), _F(1.0));
		// SAT分离轴定理
		if (!project(axisX).overlap(other.project(axisX)))
			return false;
		if (!project(axisY).overlap(other.project(axisY)))
			return false;
		if (!project(m_axisX).overlap(other.project(m_axisX)))
			return false;
		if (!project(m_axisY).overlap(other.project(m_axisY)))
			return false;
		return true;
	}
}

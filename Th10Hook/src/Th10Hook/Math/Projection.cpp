#include "Th10Hook/Math/Projection.h"

#include <algorithm>

namespace th
{
	Projection::Projection() :
		m_min(),
		m_max()
	{
	}

	Projection::Projection(float_t min, float_t max) :
		m_min(min),
		m_max(max)
	{
	}

	Projection::Projection(const BB& bb, const vec2& axis)
	{
		// a·b = |a||b|cosθ
		// |b| = 1
		// a·b = |a|cosθ
		// |a|cosθ即a在b上的投影
		float_t proj1 = bb.leftTop.dot(axis);
		float_t proj2 = bb.rightTop.dot(axis);
		float_t proj3 = bb.leftBottom.dot(axis);
		float_t proj4 = bb.rightBottom.dot(axis);

		m_min = std::min({ proj1, proj2, proj3, proj4 });
		m_max = std::max({ proj1, proj2, proj3, proj4 });
	}

	bool Projection::overlap(const Projection& other) const
	{
		//return !(m_max < other.m_min || other.m_max < m_min);
		return m_max > other.m_min && other.m_max > m_min;
	}
}

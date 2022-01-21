#include "Th10Ai/Entity/BB.h"

#include <algorithm>

namespace th
{
	Projection BB::project(const vec2& axis) const
	{
		// a·b = |a||b|cosθ
		// |b| = 1
		// a·b = |a|cosθ
		// |a|cosθ = |邻边|
		// a·b即a在b上的投影
		float_t proj1 = m_topLeft.dot(axis);
		float_t proj2 = m_topRight.dot(axis);
		float_t proj3 = m_bottomLeft.dot(axis);
		float_t proj4 = m_bottomRight.dot(axis);

		float_t min = std::min({ proj1, proj2, proj3, proj4 });
		float_t max = std::max({ proj1, proj2, proj3, proj4 });
		return Projection(min, max);
	}

	void BB::translate(const vec2& offset)
	{
		m_topLeft += offset;
		m_topRight += offset;
		m_bottomLeft += offset;
		m_bottomRight += offset;
	}
}

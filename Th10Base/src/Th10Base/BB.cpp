#include "Th10Base/BB.h"

#include <algorithm>

namespace th
{
	Projection BB::projectByNormalized(const vec2& projAxis) const
	{
		// a·b = |a||b|cosθ
		// |b| = 1
		// a·b = |a|cosθ
		// |a|cosθ = |邻边|
		// a·b即a在b上的投影
		float_t proj1 = topLeft.dot(projAxis);
		float_t proj2 = topRight.dot(projAxis);
		float_t proj3 = bottomLeft.dot(projAxis);
		float_t proj4 = bottomRight.dot(projAxis);
		float_t min = std::min({ proj1, proj2, proj3, proj4 });
		float_t max = std::max({ proj1, proj2, proj3, proj4 });
		return Projection(min, max);
	}

	Projection BB::project(const vec2& projAxis) const
	{
		return projectByNormalized(projAxis.normalize());
	}

	void BB::translate(const vec2& offset)
	{
		topLeft += offset;
		topRight += offset;
		bottomLeft += offset;
		bottomRight += offset;
	}
}

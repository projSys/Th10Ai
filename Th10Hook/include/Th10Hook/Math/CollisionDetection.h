#pragma once

#include "Th10Hook/Common.h"

#include <algorithm>

#include "Th10Hook/Math/Vector2.h"
#include "Th10Hook/Math/AABB.h"
#include "Th10Hook/Math/OBB.h"

namespace th
{
	class CollisionDetection
	{
	public:
		static bool Overlap(float_t min1, float_t max1, float_t min2, float_t max2);

		// SAT分离轴定理
		template <typename L, typename R>
		static bool CollideSAT(const L& left, const R& right, const vec2& axis)
		{
			// a·b = |a||b|cosθ
			// |b| = 1
			// a·b = |a|cosθ
			// |a|cosθ即a在b上的投影
			float_t proj1 = left.leftTop.dot(axis);
			float_t proj2 = left.rightTop.dot(axis);
			float_t proj3 = left.leftBottom.dot(axis);
			float_t proj4 = left.rightBottom.dot(axis);

			float_t proj5 = right.leftTop.dot(axis);
			float_t proj6 = right.rightTop.dot(axis);
			float_t proj7 = right.leftBottom.dot(axis);
			float_t proj8 = right.rightBottom.dot(axis);

			float_t min1 = std::min({ proj1, proj2, proj3, proj4 });
			float_t max1 = std::max({ proj1, proj2, proj3, proj4 });

			float_t min2 = std::min({ proj5, proj6, proj7, proj8 });
			float_t max2 = std::max({ proj5, proj6, proj7, proj8 });

			//return !(max1 < min2 || max2 < min1);
			return max1 > min2 && max2 > min1;
		}
	};
}

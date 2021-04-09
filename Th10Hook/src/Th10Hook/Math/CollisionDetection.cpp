#include "Th10Hook/Math/CollisionDetection.h"

namespace th
{
	bool CollisionDetection::Overlap(float_t min1, float_t max1, float_t min2, float_t max2)
	{
		//return !(max1 < min2 || max2 < min1);
		return max1 > min2 && max2 > min1;
	}
}

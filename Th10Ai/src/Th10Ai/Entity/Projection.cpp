#include "Th10Ai/Entity/Projection.h"

namespace th
{
	Projection::Projection() :
		min(),
		max()
	{
	}

	Projection::Projection(float_t min0, float_t max0) :
		min(min0),
		max(max0)
	{
	}

	bool Projection::overlap(const Projection& other) const
	{
		//return !(max < other.min || other.max < min);
		return max > other.min && other.max > min;
	}
}

#pragma once

#include "Th10Ai/Common.h"

namespace th
{
	class Projection
	{
	public:
		Projection();
		Projection(float_t min0, float_t max0);

		bool overlap(const Projection& other) const;

	public:
		float_t min;
		float_t max;
	};
}

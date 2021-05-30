#pragma once

#include "Th10Ai/Common.h"

namespace th
{
	class Projection
	{
	public:
		Projection();
		Projection(float_t min, float_t max);

		bool overlap(const Projection& other) const;

	private:
		float_t m_min;
		float_t m_max;
	};
}

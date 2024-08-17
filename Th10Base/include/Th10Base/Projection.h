#pragma once

#include "Th10Base/Common.h"

#include "Th10Base/Vector2.h"

namespace th
{
	class Projection
	{
	public:
		using value_t = vec2::value_t;

		Projection();
		Projection(value_t min0, value_t max0);

		bool overlap(const Projection& other) const;

	public:
		value_t min;
		value_t max;
	};
}

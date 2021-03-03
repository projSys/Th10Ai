#pragma once

#include "Th10Hook/Common.h"

#include <Base/TypeTraits.h>

namespace base
{
	template <typename T>
	class PointBase
	{
	public:
		using Value_t = T;
		using Traits = TypeTraits<Value_t>;

		constexpr PointBase() :
			x(), y()
		{
		}

		constexpr PointBase(Value_t x0, Value_t y0) :
			x(x0), y(y0)
		{
		}

		PointBase operator +(const PointBase& other) const
		{
			return PointBase(x + other.x, y + other.y);
		}

		PointBase operator -(const PointBase& other) const
		{
			return PointBase(x - other.x, y - other.y);
		}

		PointBase operator *(const PointBase& other) const
		{
			return PointBase(x * other.x, y * other.y);
		}

		PointBase operator /(const PointBase& other) const
		{
			return PointBase(x / other.x, y / other.y);
		}

		PointBase operator %(const PointBase& other) const
		{
			return PointBase(x % other.x, y % other.y);
		}

		bool operator ==(const PointBase& other) const
		{
			return Traits::Equal(x, other.x) && Traits::Equal(y, other.y);
		}

		bool operator !=(const PointBase& other) const
		{
			return !(*this == other);
		}

		Value_t x;
		Value_t y;
	};

	using Pointi = PointBase<int_t>;
	using Pointf = PointBase<float_t>;
	using Point = Pointi;
}

#pragma once

#include "Base/Common.h"

#include "Base/TypeTraits.h"

namespace base
{
	template <typename T>
	class PointBase
	{
	public:
		using value_t = T;
		using Traits = TypeTraits<value_t>;

		constexpr PointBase() :
			x(), y()
		{
		}

		constexpr PointBase(value_t x0, value_t y0) :
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
			return Traits::IsEqual(x, other.x) && Traits::IsEqual(y, other.y);
		}

		bool operator !=(const PointBase& other) const
		{
			return !(*this == other);
		}

		value_t x;
		value_t y;
	};

	using Pointi = PointBase<int_t>;
	using Pointf = PointBase<float_t>;
	using Point = Pointi;
}

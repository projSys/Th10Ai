#pragma once

#include "Windows/Common.h"

#include "Base/TypeTraits.h"

namespace win
{
	template <typename T, typename Traits = TypeTraits<T>>
	class PointBase
	{
	public:
		using value_t = T;

		PointBase() :
			x(), y()
		{
		}

		PointBase(T x0, T y0) :
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

		T x;
		T y;
	};

	using Pointi = PointBase<int_t>;
	using Pointf = PointBase<float_t>;
	using Point = Pointi;
}

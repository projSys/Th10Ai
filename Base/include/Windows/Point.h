#pragma once

#include "Windows/Common.h"

#include "Base/TypeTraits.h"

namespace win
{
	template <typename T, typename Traits = TypeTraits<T>>
	class PointBase
	{
	public:
		typedef T value_t;

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
			return Traits::Equals(x, other.x) && Traits::Equals(y, other.y);
		}

		bool operator !=(const PointBase& other) const
		{
			return !(*this == other);
		}

		T x;
		T y;
	};

	typedef PointBase<int_t> Pointi;
	typedef PointBase<float_t> Pointf;
	typedef Pointi Point;
}

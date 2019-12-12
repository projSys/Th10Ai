#pragma once

#include "Windows/Common.h"

namespace win
{
	template <typename T, typename Traits = TypeTraits<T>>
	class SizeBase
	{
	public:
		typedef T value_t;

		SizeBase() :
			width(), height()
		{
		}

		SizeBase(T width0, T height0) :
			width(width0), height(height0)
		{
		}

		SizeBase operator +(const SizeBase& other) const
		{
			return SizeBase(width + other.width, width + other.width);
		}

		SizeBase operator -(const SizeBase& other) const
		{
			return SizeBase(width - other.width, width - other.width);
		}

		SizeBase operator *(const SizeBase& other) const
		{
			return SizeBase(width * other.width, width * other.width);
		}

		SizeBase operator /(const SizeBase& other) const
		{
			return SizeBase(width / other.width, width / other.width);
		}

		SizeBase operator %(const SizeBase& other) const
		{
			return SizeBase(width % other.width, width % other.width);
		}

		bool operator ==(const SizeBase& other) const
		{
			return Traits::Equals(width, other.width) && Traits::Equals(height, other.height);
		}

		bool operator !=(const SizeBase& other) const
		{
			return !(*this == other);
		}

		bool isEmpty() const
		{
			return width <= T() || height <= T();
		}

		T width;
		T height;
	};

	typedef SizeBase<int_t> Sizei;
	typedef SizeBase<float_t> Sizef;
	typedef Sizei Size;
}

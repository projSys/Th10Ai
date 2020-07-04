#pragma once

#include "Windows/Common.h"

#include "Base/TypeTraits.h"

namespace win
{
	template <typename T, typename Traits = TypeTraits<T>>
	class SizeBase
	{
	public:
		using value_t = T;

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
			return Traits::IsEqual(width, other.width) && Traits::IsEqual(height, other.height);
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

	using Sizei = SizeBase<int_t>;
	using Sizef = SizeBase<float_t>;
	using Size = Sizei;
}

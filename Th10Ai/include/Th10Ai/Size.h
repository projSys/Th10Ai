#pragma once

#include "Th10Ai/Common.h"

#include <Base/TypeTraits.h>

namespace base
{
	template <typename T>
	class SizeBase
	{
	public:
		using value_t = T;
		using Traits = TypeTraits<value_t>;

		constexpr SizeBase() :
			width(), height()
		{
		}

		constexpr SizeBase(value_t width0, value_t height0) :
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
			return width <= value_t() || height <= value_t();
		}

		value_t width;
		value_t height;
	};

	using Sizei = SizeBase<int_t>;
	using Sizef = SizeBase<float_t>;
	using Size = Sizei;
}

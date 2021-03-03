#pragma once

#include "Th10Hook/Common.h"

#include <Base/TypeTraits.h>

namespace base
{
	template <typename T>
	class SizeBase
	{
	public:
		using Value_t = T;
		using Traits = TypeTraits<Value_t>;

		constexpr SizeBase() :
			width(), height()
		{
		}

		constexpr SizeBase(Value_t width0, Value_t height0) :
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
			return Traits::Equal(width, other.width) && Traits::Equal(height, other.height);
		}

		bool operator !=(const SizeBase& other) const
		{
			return !(*this == other);
		}

		bool isEmpty() const
		{
			return width <= Value_t() || height <= Value_t();
		}

		Value_t width;
		Value_t height;
	};

	using Sizei = SizeBase<int_t>;
	using Sizef = SizeBase<float_t>;
	using Size = Sizei;
}

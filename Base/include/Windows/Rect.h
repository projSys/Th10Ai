#pragma once

#include "Windows/Common.h"

#include "Base/TypeTraits.h"

#include "Windows/Point.h"
#include "Windows/Size.h"

namespace win
{
	template <typename T, typename Traits = TypeTraits<T>>
	class RectBase
	{
	public:
		using value_t = T;

		RectBase() :
			x(), y(), width(), height()
		{
		}

		RectBase(T x0, T y0, T width0, T height0) :
			x(x0), y(y0), width(width0), height(height0)
		{
		}

		RectBase(const PointBase<T>& point, const SizeBase<T>& size) :
			x(point.x), y(point.y), width(size.width), height(size.height)
		{
		}

		RectBase operator &(const RectBase& other) const
		{
			RectBase ret;
			ret.x = std::max(x, other.x);
			ret.y = std::max(y, other.y);
			ret.width = std::min(x + width, other.x + other.width) - ret.x;
			ret.height = std::min(y + height, other.y + other.height) - ret.y;
			//if (ret.isEmpty())
			//	ret = RectBase();
			return ret;
		}

		bool operator ==(const RectBase& other) const
		{
			return Traits::IsEqual(x, other.x) && Traits::IsEqual(y, other.y)
				&& Traits::IsEqual(width, other.width) && Traits::IsEqual(height, other.height);
		}

		bool operator !=(const RectBase& other) const
		{
			return !(*this == other);
		}

		bool isEmpty() const
		{
			return width <= T() || height <= T();
		}

		PointBase<T> getPoint() const
		{
			return PointBase<T>(x, y);
		}

		SizeBase<T> getSize() const
		{
			return SizeBase<T>(width, height);
		}

		T x;
		T y;
		T width;
		T height;
	};

	using Recti = RectBase<int_t>;
	using Rectf = RectBase<float_t>;
	using Rect = Recti;
}

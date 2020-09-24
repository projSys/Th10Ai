#pragma once

#include "Th10Ai/Common.h"

#include <Base/TypeTraits.h>

#include "Th10Ai/Point.h"
#include "Th10Ai/Size.h"

namespace base
{
	template <typename T>
	class RectBase
	{
	public:
		using value_t = T;
		using Traits = TypeTraits<value_t>;
		using Point = PointBase<value_t>;
		using Size = SizeBase<value_t>;

		constexpr RectBase() :
			x(), y(), width(), height()
		{
		}

		constexpr RectBase(value_t x0, value_t y0, value_t width0, value_t height0) :
			x(x0), y(y0), width(width0), height(height0)
		{
		}

		constexpr RectBase(const Point& point, const Size& size) :
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
			return width <= value_t() || height <= value_t();
		}

		Point getPoint() const
		{
			return Point(x, y);
		}

		Size getSize() const
		{
			return Size(width, height);
		}

		value_t x;
		value_t y;
		value_t width;
		value_t height;
	};

	using Recti = RectBase<int_t>;
	using Rectf = RectBase<float_t>;
	using Rect = Recti;
}

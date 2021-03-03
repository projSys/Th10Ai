#pragma once

#include "Th10Hook/Common.h"

#include <Base/TypeTraits.h>

#include "Th10Hook/Point.h"
#include "Th10Hook/Size.h"

namespace base
{
	template <typename T>
	class RectBase
	{
	public:
		using Value_t = T;
		using Traits = TypeTraits<Value_t>;
		using Point = PointBase<Value_t>;
		using Size = SizeBase<Value_t>;

		constexpr RectBase() :
			x(), y(), width(), height()
		{
		}

		constexpr RectBase(Value_t x0, Value_t y0, Value_t width0, Value_t height0) :
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
			return Traits::Equal(x, other.x) && Traits::Equal(y, other.y)
				&& Traits::Equal(width, other.width) && Traits::Equal(height, other.height);
		}

		bool operator !=(const RectBase& other) const
		{
			return !(*this == other);
		}

		bool isEmpty() const
		{
			return width <= Value_t() || height <= Value_t();
		}

		Point getPoint() const
		{
			return Point(x, y);
		}

		Size getSize() const
		{
			return Size(width, height);
		}

		Value_t x;
		Value_t y;
		Value_t width;
		Value_t height;
	};

	using Recti = RectBase<int_t>;
	using Rectf = RectBase<float_t>;
	using Rect = Recti;
}

#pragma once

#include <cstdlib>
#include <cmath>

template <typename T>
class Point2
{
public:
	typedef T value_t;

	Point2() :
		x(), y()
	{
	}

	Point2(value_t x0, value_t y0) :
		x(x0), y(y0)
	{
	}

	value_t distance(const Point2& other) const
	{
		value_t dx = x - other.x;
		value_t dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	value_t x;
	value_t y;
};

typedef Point2<float> Point2f;
typedef Point2<double> Point2d;

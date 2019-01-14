#pragma once

#include <cstdlib>
#include <cmath>

template <typename T>
class Rect2
{
public:
	typedef T value_t;

	Rect2() :
		x(), y(), w(), h()
	{
	}

	Rect2(value_t x0, value_t y0, value_t w0, value_t h0) :
		x(x0), y(y0), w(w0), h(h0)
	{
	}

	value_t distance(const Rect2& other) const
	{
		value_t dx = x - other.x;
		value_t dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	bool hitTest(const Rect2& other, value_t epsilon) const
	{
		return std::abs(x - other.x) < (w + other.w) / 2 + epsilon
			&& std::abs(y - other.y) < (h + other.h) / 2 + epsilon;
	}

	value_t x;
	value_t y;
	value_t w;
	value_t h;
};

typedef Rect2<float> Rect2f;
typedef Rect2<double> Rect2d;

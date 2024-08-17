#pragma once

#include "Th10Base/Common.h"

namespace th
{
	class Vector2
	{
	public:
		using value_t = float_t;

		constexpr Vector2() :
			x(), y() {}
		constexpr Vector2(value_t x0, value_t y0) :
			x(x0), y(y0) {}

		value_t lengthSquared() const;
		value_t length() const;
		Vector2 normalize() const;
		value_t dot(const Vector2& other) const;
		Vector2 rotate(value_t sinVal, value_t cosVal) const;
		Vector2 rotate(value_t radian) const;
		Vector2 verticalize() const;
		bool isZero() const;

		Vector2 operator -() const;
		Vector2 operator +(const Vector2& other) const;
		Vector2 operator -(const Vector2& other) const;
		Vector2 operator *(value_t scalar) const;
		Vector2 operator /(value_t scalar) const;
		Vector2& operator +=(const Vector2& other);
		Vector2& operator -=(const Vector2& other);
		Vector2& operator *=(value_t scalar);
		Vector2& operator /=(value_t scalar);

		bool operator ==(const Vector2& other) const;
		bool operator !=(const Vector2& other) const;

	public:
		value_t x;
		value_t y;
	};

	using vec2 = Vector2;
}

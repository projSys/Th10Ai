#pragma once

#include "Th10Ai/Common.h"

namespace th
{
	class Vector2
	{
	public:
		constexpr Vector2() :
			x(), y() {}
		constexpr Vector2(float_t x0, float_t y0) :
			x(x0), y(y0) {}

		float_t lengthSquared() const;
		float_t length() const;
		Vector2 normalize() const;
		float_t dot(const Vector2& other) const;
		Vector2 rotate(float_t sinVal, float_t cosVal) const;
		Vector2 rotate(float_t radian) const;
		Vector2 verticalize() const;
		Vector2 negate() const;
		bool isZero() const;

		Vector2 operator +(const Vector2& other) const;
		Vector2 operator -(const Vector2& other) const;
		Vector2 operator *(float_t scalar) const;
		Vector2 operator /(float_t scalar) const;
		Vector2& operator +=(const Vector2& other);
		Vector2& operator -=(const Vector2& other);
		Vector2& operator *=(float_t scalar);
		Vector2& operator /=(float_t scalar);

		bool operator ==(const Vector2& other) const;
		bool operator !=(const Vector2& other) const;

		float_t x;
		float_t y;
	};

	using vec2 = Vector2;
}

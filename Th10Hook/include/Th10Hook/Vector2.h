#pragma once

#include "Th10Hook/Common.h"

#include <Base/TypeTraits.h>

namespace th
{
	class Vector2
	{
	public:
		using Value_t = float_t;
		using Traits = TypeTraits<Value_t>;

		constexpr Vector2() :
			x(), y()
		{
		}

		constexpr Vector2(Value_t x0, Value_t y0) :
			x(x0), y(y0)
		{
		}

		Value_t lengthSquared() const;
		Value_t length() const;
		void normalize();
		Vector2 normalized() const;
		Value_t dot(const Vector2& other) const;
		Vector2 rotate(Value_t radian) const;

		Vector2 operator +(const Vector2& other) const;
		Vector2 operator -(const Vector2& other) const;
		Vector2 operator *(Value_t scalar) const;
		Vector2 operator /(Value_t scalar) const;
		Vector2& operator +=(const Vector2& other);
		Vector2& operator -=(const Vector2& other);
		Vector2& operator *=(Value_t scalar);
		Vector2& operator /=(Value_t scalar);

		bool operator ==(const Vector2& other) const;
		bool operator !=(const Vector2& other) const;

		Value_t x;
		Value_t y;
	};

	using vec2 = Vector2;
}

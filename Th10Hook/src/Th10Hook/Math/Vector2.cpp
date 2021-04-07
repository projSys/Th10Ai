#include "Th10Hook/Math/Vector2.h"

#include <math.h>
#include <cmath>

namespace th
{
	Vector2::Value_t Vector2::lengthSquared() const
	{
		return x * x + y * y;
	}

	Vector2::Value_t Vector2::length() const
	{
		return std::sqrt(lengthSquared());
	}

	void Vector2::normalize()
	{
		*this /= length();
	}

	Vector2 Vector2::normalized() const
	{
		return *this / length();
	}

	Vector2::Value_t Vector2::dot(const Vector2& other) const
	{
		return x * other.x + y * other.y;
	}

	Vector2 Vector2::rotate(Value_t radian) const
	{
		Value_t sinVal = std::sin(radian);
		Value_t cosVal = std::cos(radian);
		Value_t x1 = x * cosVal - y * sinVal;
		Value_t y1 = x * sinVal + y * cosVal;
		return Vector2(x1, y1);
	}

	bool Vector2::isZero() const
	{
		return *this == Vector2();
	}

	Vector2 Vector2::operator +(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator -(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 Vector2::operator *(Value_t scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 Vector2::operator /(Value_t scalar) const
	{
		return Vector2(x / scalar, y / scalar);
	}

	Vector2& Vector2::operator +=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& Vector2::operator -=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2& Vector2::operator *=(Value_t scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& Vector2::operator /=(Value_t scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	bool Vector2::operator ==(const Vector2& other) const
	{
		return Traits::Equal(x, other.x)
			&& Traits::Equal(y, other.y);
	}

	bool Vector2::operator !=(const Vector2& other) const
	{
		return !(*this == other);
	}
}

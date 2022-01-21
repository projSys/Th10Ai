#include "Th10Ai/Entity/Vector2.h"

#include <math.h>
#include <cmath>
#include <Base/Traits.h>

namespace th
{
	float_t Vector2::lengthSquared() const
	{
		return x * x + y * y;
	}

	float_t Vector2::length() const
	{
		return std::sqrt(lengthSquared());
	}

	Vector2 Vector2::normalize() const
	{
		return *this / length();
	}

	float_t Vector2::dot(const Vector2& other) const
	{
		return x * other.x + y * other.y;
	}

	Vector2 Vector2::rotate(float_t sinVal, float_t cosVal) const
	{
		float_t x1 = x * cosVal - y * sinVal;
		float_t y1 = x * sinVal + y * cosVal;
		return Vector2(x1, y1);
	}

	Vector2 Vector2::rotate(float_t radian) const
	{
		float_t sinVal = std::sin(radian);
		float_t cosVal = std::cos(radian);
		return rotate(sinVal, cosVal);
	}

	Vector2 Vector2::verticalize() const
	{
		return Vector2(-y, x);
	}

	bool Vector2::isZero() const
	{
		return *this == Vector2();
	}

	Vector2 Vector2::operator -() const
	{
		return Vector2(-x, -y);
	}

	Vector2 Vector2::operator +(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator -(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 Vector2::operator *(float_t scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 Vector2::operator /(float_t scalar) const
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

	Vector2& Vector2::operator *=(float_t scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& Vector2::operator /=(float_t scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	bool Vector2::operator ==(const Vector2& other) const
	{
		return float_equal(x, other.x)
			&& float_equal(y, other.y);
	}

	bool Vector2::operator !=(const Vector2& other) const
	{
		return !(*this == other);
	}
}

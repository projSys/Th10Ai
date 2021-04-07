#include "Th10Hook/Laser.h"

#include <math.h>
#include <cmath>

#include "Th10Hook/Math/AABB.h"
#include "Th10Hook/Math/OBB.h"

namespace th
{
	Laser::Laser() :
		arc()
	{
	}

	Laser::Laser(const vec2& pos0, const vec2& delta0, const vec2& size0, float_t arc0) :
		pos(pos0), delta(delta0), size(size0), arc(arc0)
	{
	}

	float_t Laser::distance(const Entity& other) const
	{
		return (pos - other.pos).length();
	}

	bool Laser::collide(const Entity& other) const
	{
		return OBB(*this).collide(AABB(other));
	}

	vec2 Laser::getFootPoint(const Entity& other) const
	{
		if (delta.isZero())
			return pos;
		vec2 unit = delta.normalized();
		return pos + unit * (other.pos - pos).dot(unit);
	}

	bool Laser::willCollideWith(const Entity& other) const
	{
		Laser temp = *this;
		temp.pos = getFootPoint(other);
		return temp.collide(other);
	}

	bool Laser::isHolding() const
	{
		return delta.isZero();
	}

	vec2 Laser::getNextPos() const
	{
		return pos + delta;
	}

	void Laser::advance(float_t frame)
	{
		pos += (delta * frame);
	}

	vec2 Laser::getCenter() const
	{
		return vec2(pos.x, pos.y + size.y / 2);
	}

	vec2 Laser::getLeftTop() const
	{
		return vec2(pos.x - size.x / 2, pos.y);
	}

	vec2 Laser::getRightTop() const
	{
		return vec2(pos.x + size.x / 2, pos.y);
	}

	vec2 Laser::getLeftBottom() const
	{
		return vec2(pos.x - size.x / 2, pos.y + size.y);
	}

	vec2 Laser::getRightBottom() const
	{
		return vec2(pos.x + size.x / 2, pos.y + size.y);
	}

	float_t Laser::getRadian() const
	{
		// emmm...ÄãËµÕâ¸öË­¶®°¡£¿
		float_t radian = arc - static_cast<float_t>(M_PI) * 5 / 2;
		return radian;
	}
}

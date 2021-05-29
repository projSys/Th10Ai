#include "Th10Ai/Laser.h"

#include <math.h>
#include <cmath>

#include "Th10Ai/Entity.h"

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

	void Laser::updateExtra()
	{
		obb.update(*this);
		if (!delta.isZero())
		{
			deltaV = delta.verticalize().normalize();
			projV = Projection(obb, deltaV);
		}
	}

	bool Laser::collide(const Entity& other) const
	{
		return OBB(*this).collide(AABB(other));
	}

	bool Laser::willCollideWith(const Entity& other) const
	{
		if (!delta.isZero())
		{
			//vec2 deltaV = delta.verticalize().normalize();
			//return Projection(OBB(*this), deltaV).overlap(
			//	Projection(AABB(other), deltaV));
			return projV.overlap(Projection(other.aabb, deltaV));
		}
		else
		{
			return obb.collide(other.aabb);
		}
	}

	float_t Laser::distance(const Entity& other) const
	{
		return (pos - other.pos).length();
	}

	bool Laser::isHolding() const
	{
		return delta.isZero();
	}

	void Laser::advance(int_t frame)
	{
		pos += (delta * static_cast<float_t>(frame));
	}

	vec2 Laser::getTopLeft() const
	{
		return vec2(pos.x - size.x / _F(2.0), pos.y);
	}

	vec2 Laser::getTopRight() const
	{
		return vec2(pos.x + size.x / _F(2.0), pos.y);
	}

	vec2 Laser::getBottomLeft() const
	{
		return vec2(pos.x - size.x / _F(2.0), pos.y + size.y);
	}

	vec2 Laser::getBottomRight() const
	{
		return vec2(pos.x + size.x / _F(2.0), pos.y + size.y);
	}

	float_t Laser::getRadian() const
	{
		// emmm...ÄãËµÕâ¸öË­¶®°¡£¿
		float_t radian = arc - static_cast<float_t>(M_PI) * _F(5.0) / _F(2.0);
		return radian;
	}
}

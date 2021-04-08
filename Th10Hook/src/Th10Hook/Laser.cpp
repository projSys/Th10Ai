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

	void Laser::updateExtra()
	{
		if (!delta.isZero())
			deltaV = delta.verticalize().normalize();
	}

	float_t Laser::distance(const Entity& other) const
	{
		return (pos - other.pos).length();
	}

	bool Laser::collide(const Entity& other) const
	{
		return OBB(*this).collide(AABB(other));
	}

	bool Overlap1(const OBB& left, const AABB& right, const vec2& axis)
	{
		// a·b = |a||b|cosθ
		// |b| = 1
		// a·b = |a|cosθ
		// |a|cosθ即a在b上的投影
		float_t proj1 = left.leftTop.dot(axis);
		float_t proj2 = left.rightTop.dot(axis);
		float_t proj3 = left.leftBottom.dot(axis);
		float_t proj4 = left.rightBottom.dot(axis);

		float_t proj5 = right.leftTop.dot(axis);
		float_t proj6 = right.rightTop.dot(axis);
		float_t proj7 = right.leftBottom.dot(axis);
		float_t proj8 = right.rightBottom.dot(axis);

		float_t min1 = std::min({ proj1, proj2, proj3, proj4 });
		float_t max1 = std::max({ proj1, proj2, proj3, proj4 });

		float_t min2 = std::min({ proj5, proj6, proj7, proj8 });
		float_t max2 = std::max({ proj5, proj6, proj7, proj8 });

		//return !(max1 < min2 || max2 < min1);
		return max1 > min2 && max2 > min1;
	}

	bool Laser::willCollideWith(const Entity& other) const
	{
		if (delta.isZero())
			return collide(other);

		return Overlap1(OBB(*this), AABB(other), deltaV);
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
		// emmm...你说这个谁懂啊？
		float_t radian = arc - static_cast<float_t>(M_PI) * 5 / 2;
		return radian;
	}
}

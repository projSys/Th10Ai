#include "Th10Hook/Math/OBB.h"

#include "Th10Hook/Math/AABB.h"
#include "Th10Hook/Laser.h"

namespace th
{
	OBB::OBB(const Laser& laser)
	{
		float_t radian = laser.getRadian();
		leftTop = laser.pos + (laser.getLeftTop() - laser.pos).rotate(radian);
		rightTop = laser.pos + (laser.getRightTop() - laser.pos).rotate(radian);
		leftBottom = laser.pos + (laser.getLeftBottom() - laser.pos).rotate(radian);
		rightBottom = laser.pos + (laser.getRightBottom() - laser.pos).rotate(radian);
		axisX = vec2(1, 0).rotate(radian);
		axisY = vec2(0, 1).rotate(radian);
	}

	bool Overlap(const OBB& left, const AABB& right, const vec2& axis)
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

	bool OBB::collide(const AABB& other) const
	{
		if (!Overlap(*this, other, other.axisX))
			return false;
		if (!Overlap(*this, other, other.axisY))
			return false;
		if (!Overlap(*this, other, axisX))
			return false;
		if (!Overlap(*this, other, axisY))
			return false;
		return true;
	}
}

#include "Th10Hook/Math/OBB.h"

#include "Th10Hook/Laser.h"
#include "Th10Hook/Math/AABB.h"
#include "Th10Hook/Math/CollisionDetection.h"

namespace th
{
	OBB::OBB(const Laser& laser)
	{
		update(laser);
	}

	void OBB::update(const Laser& laser)
	{
		float_t radian = laser.getRadian();
		leftTop = laser.pos + (laser.getLeftTop() - laser.pos).rotate(radian);
		rightTop = laser.pos + (laser.getRightTop() - laser.pos).rotate(radian);
		leftBottom = laser.pos + (laser.getLeftBottom() - laser.pos).rotate(radian);
		rightBottom = laser.pos + (laser.getRightBottom() - laser.pos).rotate(radian);
		axisX = vec2(1, 0).rotate(radian);
		axisY = vec2(0, 1).rotate(radian);
	}

	bool OBB::collide(const AABB& other) const
	{
		if (!CollisionDetection::CollideSAT(*this, other, other.axisX))
			return false;
		if (!CollisionDetection::CollideSAT(*this, other, other.axisY))
			return false;
		if (!CollisionDetection::CollideSAT(*this, other, axisX))
			return false;
		if (!CollisionDetection::CollideSAT(*this, other, axisY))
			return false;
		return true;
	}
}

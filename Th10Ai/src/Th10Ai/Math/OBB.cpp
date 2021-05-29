#include "Th10Ai/Math/OBB.h"

#include "Th10Ai/Laser.h"
#include "Th10Ai/Math/AABB.h"
#include "Th10Ai/Math/Projection.h"

namespace th
{
	OBB::OBB(const Laser& laser)
	{
		update(laser);
	}

	void OBB::update(const Laser& laser)
	{
		float_t radian = laser.getRadian();
		topLeft = laser.pos + (laser.getTopLeft() - laser.pos).rotate(radian);
		topRight = laser.pos + (laser.getTopRight() - laser.pos).rotate(radian);
		bottomLeft = laser.pos + (laser.getBottomLeft() - laser.pos).rotate(radian);
		bottomRight = laser.pos + (laser.getBottomRight() - laser.pos).rotate(radian);
		axisX = vec2(_F(1.0), _F(0.0)).rotate(radian);
		axisY = vec2(_F(0.0), _F(1.0)).rotate(radian);
	}

	bool OBB::collide(const AABB& other) const
	{
		// SAT∑÷¿Î÷·∂®¿Ì
		if (!Projection(*this, other.axisX).overlap(Projection(other, other.axisX)))
			return false;
		if (!Projection(*this, other.axisY).overlap(Projection(other, other.axisY)))
			return false;
		if (!Projection(*this, axisX).overlap(Projection(other, axisX)))
			return false;
		if (!Projection(*this, axisY).overlap(Projection(other, axisY)))
			return false;
		return true;
	}
}

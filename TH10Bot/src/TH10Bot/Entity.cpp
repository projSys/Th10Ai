#include "TH10Bot/Common.h"
#include "TH10Bot/Entity.h"

#include "TH10Bot/MyMath.h"

namespace th
{
	float_t Entity::distance(const Pointf& pos) const
	{
		return MyMath::Distance(getPos(), pos);
	}

	float_t Entity::distance(const Entity& other) const
	{
		return MyMath::Distance(getPos(), other.getPos());
	}

	float_t Entity::angle(const Pointf& pos) const
	{
		//if (isResting())
		//	return -1.0f;
		return MyMath::Angle(getPos(), getNextPos(), pos);
	}

	float_t Entity::angle(const Entity& other) const
	{
		//if (isResting())
		//	return -1.0f;
		return MyMath::Angle(getPos(), getNextPos(), other.getPos());
	}

	// 点到直线的垂足
	//Pointf Entity::footPoint(const Pointf& pos) const
	//{
	//	//if (isResting())
	//	//	return Pointf(x, y);
	//	//Pointf nextPos = getNextPos();
	//	//float_t u = (other.x - x) * (nextPos.x - x) + (other.y - y) * (nextPos.y - y);
	//	float_t u = (pos.x - x) * dx + (pos.y - y) * dy;
	//	u /= (dx * dx + dy * dy);
	//	return Pointf(x + u * dx, y + u * dy);
	//}

	float_t Entity::footFrame(const Pointf& pos) const
	{
		return ((pos.x - x) * dx + (pos.y - y) * dy) / (dx * dx + dy * dy);
	}

	Pointf Entity::footPoint(float_t footFrame) const
	{
		return Pointf(x + dx * footFrame, y + dy * footFrame);
	}

	Pointf Entity::advanceTo(int_t frame) const
	{
		return Pointf(x + dx * frame, y + dy * frame);
	}

	bool Entity::collide(const Entity& other, float_t epsilon) const
	{
		return std::abs(x - other.x) < (width + other.width) / 2.0f + epsilon
			&& std::abs(y - other.y) < (height + other.height) / 2.0f + epsilon;
	}

	bool Entity::collide2(const Entity& other) const
	{
		return getBottomRight() > other.getTopLeft() && other.getBottomRight() > getTopLeft();
	}

	Pointf Entity::getPos() const
	{
		return Pointf(x, y);
	}

	void Entity::setPos(const Pointf& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	Pointf Entity::getTopLeft() const
	{
		return Pointf(x - width / 2.0f, y - height / 2.0f);
	}

	Pointf Entity::getBottomRight() const
	{
		return Pointf(x + width / 2.0f, y + height / 2.0f);
	}

	Sizef Entity::getSize() const
	{
		return Sizef(width, height);
	}

	Rectf Entity::getRect() const
	{
		return Rectf(getTopLeft(), getSize());
	}

	bool Entity::isResting() const
	{
		return TypeTraits<float_t>::IsZero(dx) && TypeTraits<float_t>::IsZero(dy);
	}

	Pointf Entity::getNextPos() const
	{
		return Pointf(x + dx, y + dy);
	}



	Pointf Player::getNextPos(int_t dir, bool slow) const
	{
		return Pointf(x, y);
	}

	bool Player::collideSAT(const Laser& laser, float_t epsilon) const
	{
		LaserBox laserBox(laser);
		if (!laserBox.collideSAT(*this, epsilon))
			return false;

		PlayerBox playerBox(*this, laser);
		if (!playerBox.collideSAT(laser, epsilon))
			return false;

		return true;
	}



	Bullet Bullet::advance() const
	{
		return Bullet(x + dx, y + dy, width, height, dx, dy);
	}



	void SATBox::PointRotate(float_t& x, float_t& y, float_t cx, float_t cy, float_t radian)
	{
		// 设平面上一点（x,y），绕另一点（cx,cy）逆时针旋转A角度后的点为（x0,y0）,则：
		float_t x0 = (x - cx) * std::cos(radian) - (y - cy) * std::sin(radian) + cx;
		float_t y0 = (x - cx) * std::sin(radian) + (y - cy) * std::cos(radian) + cy;
		x = x0;
		y = y0;
	}

	bool SATBox::Collide(float_t c1, float_t d1, float_t c2, float_t d2, float_t epsilon)
	{
		return std::abs(c1 - c2) < (d1 + d2) / 2.0 + epsilon;
	}

	LaserBox::LaserBox(const Laser& laser)
	{
		x1 = laser.x - laser.width / 2.0; y1 = laser.y;
		x2 = laser.x + laser.width / 2.0; y2 = laser.y;
		x3 = laser.x - laser.width / 2.0; y3 = laser.y + laser.height;
		x4 = laser.x + laser.width / 2.0; y4 = laser.y + laser.height;
		// emmm?
		float_t radian = laser.radian - 3.14159265358979323846 * 5.0 / 2.0;
		PointRotate(x1, y1, laser.x, laser.y, radian);
		PointRotate(x2, y2, laser.x, laser.y, radian);
		PointRotate(x3, y3, laser.x, laser.y, radian);
		PointRotate(x4, y4, laser.x, laser.y, radian);
	}

	// 分离轴定理
	bool LaserBox::collideSAT(const Player& player, float_t epsilon) const
	{
		float_t xMin = std::min(std::min(x1, x2), std::min(x3, x4));
		float_t xMax = std::max(std::max(x1, x2), std::max(x3, x4));
		if (!Collide(xMin + (xMax - xMin) / 2.0, xMax - xMin, player.x, player.width, epsilon))
			return false;

		float_t yMin = std::min(std::min(y1, y2), std::min(y3, y4));
		float_t yMax = std::max(std::max(y1, y2), std::max(y3, y4));
		if (!Collide(yMin + (yMax - yMin) / 2.0, yMax - yMin, player.y, player.height, epsilon))
			return false;

		return true;
	}

	// 反向旋转画布，即把激光转回来，再反向旋转自机坐标
	PlayerBox::PlayerBox(const Player& player, const Laser& laser)
	{
		x1 = player.x - player.width / 2.0; y1 = player.y - player.height / 2.0;
		x2 = player.x + player.width / 2.0; y2 = player.y - player.height / 2.0;
		x3 = player.x - player.width / 2.0; y3 = player.y + player.height / 2.0;
		x4 = player.x + player.width / 2.0; y4 = player.y + player.height / 2.0;
		// emmm?
		float_t radian = laser.radian - 3.14159265358979323846 * 5.0 / 2.0;
		PointRotate(x1, y1, laser.x, laser.y, -radian);
		PointRotate(x2, y2, laser.x, laser.y, -radian);
		PointRotate(x3, y3, laser.x, laser.y, -radian);
		PointRotate(x4, y4, laser.x, laser.y, -radian);
	}

	// 分离轴定理
	bool PlayerBox::collideSAT(const Laser& laser, float_t epsilon) const
	{
		float_t xMin = std::min(std::min(x1, x2), std::min(x3, x4));
		float_t xMax = std::max(std::max(x1, x2), std::max(x3, x4));
		if (!Collide(xMin + (xMax - xMin) / 2.0, xMax - xMin, laser.x, laser.width, epsilon))
			return false;

		float_t yMin = std::min(std::min(y1, y2), std::min(y3, y4));
		float_t yMax = std::max(std::max(y1, y2), std::max(y3, y4));
		if (!Collide(yMin + (yMax - yMin) / 2.0, yMax - yMin, laser.y + laser.height / 2.0, laser.height, epsilon))
			return false;

		return true;
	}
}

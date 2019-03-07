#include "TH10Bot/Common.h"
#include "TH10Bot/Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

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
		if (isResting())
			return -1.0f;

		return MyMath::Angle(getPos(), getNextPos(), pos);
	}

	float_t Entity::angle(const Entity& other) const
	{
		if (isResting())
			return -1.0f;

		return MyMath::Angle(getPos(), getNextPos(), other.getPos());
	}

	float_t Entity::footFrame(const Pointf& pos) const
	{
		if (isResting())
			return 0.0f;

		return ((pos.x - x) * dx + (pos.y - y) * dy) / (dx * dx + dy * dy);
	}

	Pointf Entity::footPoint(float_t footFrame) const
	{
		return Pointf(x + dx * footFrame, y + dy * footFrame);
	}

	Direction Entity::direction() const
	{
		if (isResting())
			return DIR_NONE;

		Pointf xPos = getPos();
		xPos.x += SCENE_SIZE.width;	// 按X轴平移
		float_t angleOfXAxis = angle(xPos);
		if (dy > 0.0f)	// 转换成360度
			angleOfXAxis = 360.0f - angleOfXAxis;

		Direction dir;
		if (angleOfXAxis > 337.5f)
			dir = DIR_RIGHT;
		else if (angleOfXAxis > 292.5f)
			dir = DIR_DOWNRIGHT;
		else if (angleOfXAxis > 247.5f)
			dir = DIR_DOWN;
		else if (angleOfXAxis > 202.5f)
			dir = DIR_DOWNLEFT;
		else if (angleOfXAxis > 157.5f)
			dir = DIR_LEFT;
		else if (angleOfXAxis > 112.5f)
			dir = DIR_UPLEFT;
		else if (angleOfXAxis > 67.5f)
			dir = DIR_UP;
		else if (angleOfXAxis > 22.5f)
			dir = DIR_UPRIGHT;
		else
			dir = DIR_RIGHT;
		return dir;
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

	bool Entity::isResting() const
	{
		return TypeTraits<float_t>::IsZero(dx) && TypeTraits<float_t>::IsZero(dy);
	}

	Pointf Entity::getNextPos() const
	{
		return Pointf(x + dx, y + dy);
	}

	Sizef Entity::getSize() const
	{
		return Sizef(width, height);
	}

	Rectf Entity::getRect() const
	{
		return Rectf(getTopLeft(), getSize());
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
		return std::abs(c1 - c2) < (d1 + d2) / 2.0f + epsilon;
	}

	LaserBox::LaserBox(const Laser& laser)
	{
		x1 = laser.x - laser.width / 2.0f; y1 = laser.y;
		x2 = laser.x + laser.width / 2.0f; y2 = laser.y;
		x3 = laser.x - laser.width / 2.0f; y3 = laser.y + laser.height;
		x4 = laser.x + laser.width / 2.0f; y4 = laser.y + laser.height;
		// emmm?
		float_t radian = laser.radian - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
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
		if (!Collide(xMin + (xMax - xMin) / 2.0f, xMax - xMin, player.x, player.width, epsilon))
			return false;

		float_t yMin = std::min(std::min(y1, y2), std::min(y3, y4));
		float_t yMax = std::max(std::max(y1, y2), std::max(y3, y4));
		if (!Collide(yMin + (yMax - yMin) / 2.0f, yMax - yMin, player.y, player.height, epsilon))
			return false;

		return true;
	}

	// 反向旋转画布，即把激光转回来，再反向旋转自机坐标
	PlayerBox::PlayerBox(const Player& player, const Laser& laser)
	{
		x1 = player.x - player.width / 2.0f; y1 = player.y - player.height / 2.0f;
		x2 = player.x + player.width / 2.0f; y2 = player.y - player.height / 2.0f;
		x3 = player.x - player.width / 2.0f; y3 = player.y + player.height / 2.0f;
		x4 = player.x + player.width / 2.0f; y4 = player.y + player.height / 2.0f;
		// emmm?
		float_t radian = laser.radian - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
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
		if (!Collide(xMin + (xMax - xMin) / 2.0f, xMax - xMin, laser.x, laser.width, epsilon))
			return false;

		float_t yMin = std::min(std::min(y1, y2), std::min(y3, y4));
		float_t yMax = std::max(std::max(y1, y2), std::max(y3, y4));
		if (!Collide(yMin + (yMax - yMin) / 2.0f, yMax - yMin, laser.y + laser.height / 2.0f, laser.height, epsilon))
			return false;

		return true;
	}
}

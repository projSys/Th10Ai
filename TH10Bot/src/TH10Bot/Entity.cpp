#include "TH10Bot/Common.h"
#include "TH10Bot/Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "TH10Bot/MyMath.h"

namespace th
{
	float_t Entity::getDistance(const Pointf& other) const
	{
		return MyMath::Distance(getPosition(), other);
	}

	float_t Entity::getDistance(const Entity& other) const
	{
		return getDistance(other.getPosition());
	}

	float_t Entity::getAngle(const Pointf& other) const
	{
		if (isHolding())
			return -1.0f;

		return MyMath::Angle(getPosition(), getNextPos(), other);
	}

	float_t Entity::getAngle(const Entity& other) const
	{
		return getAngle(other.getPosition());
	}

	FootPoint Entity::getFootPoint(const Pointf& other) const
	{
		if (isHolding())
			return { x, y, std::numeric_limits<float_t>::max() };

		float_t scale = ((other.x - x) * dx + (other.y - y) * dy) / (dx * dx + dy * dy);
		return { x + dx * scale, y + dy * scale, scale };
	}

	FootPoint Entity::getFootPoint(const Entity& other) const
	{
		return getFootPoint(other.getPosition());
	}

	Direction Entity::getDirection() const
	{
		if (isHolding())
			return DIR_HOLD;

		Pointf xAxis(x + 100.0f, y);	// 按X轴平移
		float_t angleOfXAxis = getAngle(xAxis);
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

	bool Entity::collide(const Entity& other) const
	{
		return std::abs(x - other.x) < (width + other.width) / 2.0f
			&& std::abs(y - other.y) < (height + other.height) / 2.0f;
		//return getBottomRight() > other.getTopLeft()
		//	&& other.getBottomRight() > getTopLeft();
	}

	bool Entity::collide(const Entity& other, float_t frame) const
	{
		Entity temp(x + dx * frame, y + dy * frame, dx, dy, width, height);
		return temp.collide(other);
	}

	float_t Entity::willCollideWith(const Entity& other) const
	{
		FootPoint footPoint = getFootPoint(other);
		Entity temp(footPoint.x, footPoint.y, dx, dy, width, height);
		if (temp.collide(other))
			return footPoint.frame;
		else
			return std::numeric_limits<float_t>::lowest();
	}

	float_t Entity::willCollideWith(const Entity& other, float_t frame) const
	{
		Entity temp(x + dx * frame, y + dy * frame, dx, dy, width, height);
		return temp.willCollideWith(other);
	}

	Pointf Entity::getPosition() const
	{
		return Pointf(x, y);
	}

	void Entity::setPosition(const Pointf& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	Pointf Entity::getTopLeft() const
	{
		return Pointf(x - width / 2.0f, y - height / 2.0f);
	}

	Pointf Entity::getTopRight() const
	{
		return Pointf(x + width / 2.0f, y - height / 2.0f);
	}

	Pointf Entity::getBottomLeft() const
	{
		return Pointf(x - width / 2.0f, y + height / 2.0f);
	}

	Pointf Entity::getBottomRight() const
	{
		return Pointf(x + width / 2.0f, y + height / 2.0f);
	}

	bool Entity::isHolding() const
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



	bool Player::collideSAT(const Laser& laser) const
	{
		LaserBox laserBox(laser);
		if (!laserBox.collideSAT(*this))
			return false;

		PlayerBox playerBox(*this, laser);
		if (!playerBox.collideSAT(laser))
			return false;

		return true;
	}



	Pointf Laser::getTopLeft() const
	{
		return Pointf(x - width / 2.0f, y);
	}

	Pointf Laser::getTopRight() const
	{
		return Pointf(x + width / 2.0f, y);
	}

	Pointf Laser::getBottomLeft() const
	{
		return Pointf(x - width / 2.0f, y + height);
	}

	Pointf Laser::getBottomRight() const
	{
		return Pointf(x + width / 2.0f, y + height);
	}



	bool SATBox::Collide(float_t p1, float_t l1, float_t p2, float_t l2)
	{
		return std::abs(p1 - p2) < (l1 + l2) / 2.0f;
	}

	LaserBox::LaserBox(const Laser& laser)
	{
		Pointf C = laser.getPosition();
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = MyMath::Rotate(laser.getTopLeft(), C, radianC);
		topRight = MyMath::Rotate(laser.getTopRight(), C, radianC);
		bottomLeft = MyMath::Rotate(laser.getBottomLeft(), C, radianC);
		bottomRight = MyMath::Rotate(laser.getBottomRight(), C, radianC);
	}

	// 分离轴定理
	bool LaserBox::collideSAT(const Player& player) const
	{
		// 投影到X轴
		float_t minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
		float_t maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
		// 检测2条线段是否重叠
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, player.x, player.width))
			return false;

		// 投影到Y轴
		float_t minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
		float_t maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));
		// 检测2条线段是否重叠
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, player.y, player.height))
			return false;

		return true;
	}

	// 反向旋转画布，即把激光转回来，再反向旋转自机坐标
	PlayerBox::PlayerBox(const Player& player, const Laser& laser)
	{
		Pointf C = laser.getPosition();
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = MyMath::Rotate(player.getTopLeft(), C, -radianC);
		topRight = MyMath::Rotate(player.getTopRight(), C, -radianC);
		bottomLeft = MyMath::Rotate(player.getBottomLeft(), C, -radianC);
		bottomRight = MyMath::Rotate(player.getBottomRight(), C, -radianC);
	}

	// 分离轴定理
	bool PlayerBox::collideSAT(const Laser& laser) const
	{
		// 投影到X轴
		float_t minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
		float_t maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
		// 检测2条线段是否重叠
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, laser.x, laser.width))
			return false;

		// 投影到Y轴
		float_t minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
		float_t maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));
		// 检测2条线段是否重叠
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, laser.y + laser.height / 2.0f, laser.height))
			return false;

		return true;
	}
}

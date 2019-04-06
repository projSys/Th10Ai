#include "TH10Bot/Common.h"
#include "TH10Bot/Entity/Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "TH10Bot/MyMath.h"

namespace th
{
	float_t Entity::getDistance(const Pointf& other) const
	{
		return MyMath::GetDistance(getPosition(), other);
	}

	float_t Entity::getDistance(const Entity& other) const
	{
		return getDistance(other.getPosition());
	}

	float_t Entity::getAngle(const Pointf& other) const
	{
		if (isHolding())
			return -1.0f;

		return MyMath::GetAngle(getPosition(), getNextPos(), other);
	}

	float_t Entity::getAngle(const Entity& other) const
	{
		return getAngle(other.getPosition());
	}

	FootPoint Entity::getFootPoint(const Pointf& other) const
	{
		if (isHolding())
			return { x, y, std::numeric_limits<float_t>::max() };

		return MyMath::GetFootPoint(getPosition(), getNextPos(), other);;
	}

	FootPoint Entity::getFootPoint(const Entity& other) const
	{
		return getFootPoint(other.getPosition());
	}

	Direction Entity::getDirection() const
	{
		if (isHolding())
			return DIR_HOLD;

		float_t angleOfXAxis = getAngle(Pointf(x + 100.0f, y));	// 按X轴平移
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
		Entity temp = { x + dx * frame, y + dy * frame, dx, dy, width, height };
		return temp.collide(other);
	}

	float_t Entity::willCollideWith(const Entity& other) const
	{
		FootPoint footPoint = getFootPoint(other);
		Entity temp = { footPoint.x, footPoint.y, dx, dy, width, height };
		if (temp.collide(other))
			return footPoint.frame;
		else
			return std::numeric_limits<float_t>::lowest();
	}

	float_t Entity::willCollideWith(const Entity& other, float_t frame) const
	{
		Entity temp = { x + dx * frame, y + dy * frame, dx, dy, width, height };
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
		return dx == 0.0f && dy == 0.0f;
	}

	Pointf Entity::getDelta() const
	{
		return Pointf(dx, dy);
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
}

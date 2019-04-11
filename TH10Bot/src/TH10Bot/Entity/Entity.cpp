#include "TH10Bot/Common.h"
#include "TH10Bot/Entity/Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "TH10Bot/MyMath.h"

namespace th
{
	float_t Entity::getDist(const Entity& other) const
	{
		return getPos().distance(other.getPos());
	}

	float_t Entity::getAngle(const Entity& other) const
	{
		if (isHolded())
			return -1.0f;

		return MyMath::GetAngle(getPos(), getNextPos(), other.getPos());
	}

	FootPoint Entity::getFootPoint(const Entity& other) const
	{
		if (isHolded())
			return { x, y, std::numeric_limits<float_t>::max() };

		return MyMath::GetFootPoint(getPos(), getNextPos(), other.getPos());
	}

	Direction Entity::getDir() const
	{
		if (isHolded())
			return DIR_HOLD;

		// 与X轴的角度
		float_t angleOfXAxis = MyMath::GetAngle(getPos(), getNextPos(), Pointf(x + 100.0f, y));
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

	Entity Entity::advance(float_t frame) const
	{
		Entity adv = *this;
		adv.x += (dx * frame);
		adv.y += (dy * frame);
		return adv;
	}

	bool Entity::collide(const Entity& other) const
	{
		return std::abs(x - other.x) < (width + other.width) / 2.0f
			&& std::abs(y - other.y) < (height + other.height) / 2.0f;
	}

	float_t Entity::willCollideWith(const Entity& other) const
	{
		FootPoint footPoint = getFootPoint(other);

		Entity temp = *this;
		temp.x = footPoint.x;
		temp.y = footPoint.y;

		if (temp.collide(other))
			return footPoint.frame;
		else
			return std::numeric_limits<float_t>::lowest();
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

	bool Entity::isHolded() const
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

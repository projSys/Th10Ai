#include "TH10Bot/Common.h"
#include "TH10Bot/Entity/Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "TH10Bot/MyMath.h"

namespace th
{
	float_t Entity::getDist(const Pointf& pos) const
	{
		return getPos().distance(pos);
	}

	FootPoint Entity::getFootPoint(const Pointf& pos) const
	{
		if (isHolded())
			return { x, y, std::numeric_limits<float_t>::max() };

		// 点到前进方向的垂足
		float_t ratio = ((pos.x - x) * dx + (pos.y - y) * dy) / (dx * dx + dy * dy);
		return { x + dx * ratio, y + dy * ratio, ratio };
	}

	float_t Entity::getAngle(const Pointf& pos) const
	{
		if (isHolded() || getPos() == pos)
			return -1.0f;

		return MyMath::GetAngle(getPos(), getNextPos(), pos);
	}

	Direction Entity::getDir() const
	{
		if (isHolded())
			return DIR_HOLD;

		// 前进方向与X轴正方向的角度
		float_t angle = MyMath::GetAngle(getPos(), getNextPos(), Pointf(x + 100.0f, y));
		if (dy > 0.0f)	// 转换成360度
			angle = 360.0f - angle;

		// 22.5 = 360 / 8 / 2
		int_t sector = static_cast<int_t>(angle / 22.5f);
		assert(sector >= 0 && sector <= 16);
		return SECTOR_TO_DIR[sector];
	}

	Direction Entity::getDir(const Pointf& pos) const
	{
		if (getPos() == pos)
			return DIR_HOLD;

		// pos与X轴正方向的角度
		float_t angle = MyMath::GetAngle(getPos(), pos, Pointf(x + 100.0f, y));
		if (pos.y - y > 0.0f)	// 转换成360度
			angle = 360.0f - angle;

		// 22.5 = 360 / 8 / 2
		int_t sector = static_cast<int_t>(angle / 22.5f);
		assert(sector >= 0 && sector <= 16);
		return SECTOR_TO_DIR[sector];
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
		FootPoint footPoint = getFootPoint(other.getPos());

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
		return dx <= std::numeric_limits<float_t>::epsilon()
			&& dy <= std::numeric_limits<float_t>::epsilon();
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

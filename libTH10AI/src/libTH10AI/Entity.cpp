#include "libTH10AI/Common.h"
#include "libTH10AI/Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace th
{
	// 与X轴正方向的角度扇区转换成移动方向
	const Direction Entity::SECTOR_TO_DIR[17] =
	{
		DIR_RIGHT,		// 0
		DIR_RIGHTUP,
		DIR_RIGHTUP,
		DIR_UP,
		DIR_UP,
		DIR_LEFTUP,
		DIR_LEFTUP,
		DIR_LEFT,
		DIR_LEFT,
		DIR_LEFTDOWN,
		DIR_LEFTDOWN,
		DIR_DOWN,
		DIR_DOWN,
		DIR_RIGHTDOWN,
		DIR_RIGHTDOWN,
		DIR_RIGHT,
		DIR_RIGHT		// 360
	};

	// 勾股定理
	float_t Entity::CalcDistance(const Pointf& A, const Pointf& B)
	{
		float_t dx = A.x - B.x;
		float_t dy = A.y - B.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	// 余弦定理
	float_t Entity::CalcAngle(const Pointf& A, const Pointf& B, const Pointf& C)
	{
		float_t AB = CalcDistance(A, B);
		float_t AC = CalcDistance(A, C);
		float_t BC = CalcDistance(B, C);
		if (AB == 0.0f || AC == 0.0f)
			return -1.0f;

		float_t cosA = (AB * AB + AC * AC - BC * BC) / (2.0f * AB * AC);
		if (cosA < -1.0f)
			cosA = -1.0f;
		if (cosA > 1.0f)
			cosA = 1.0f;

		float_t radianA = std::acos(cosA);
		// 角度 = 弧度 * 180 / PI
		return radianA * 180.0f / static_cast<float_t>(M_PI);
	}

	Entity::Entity() :
		x(), y(), dx(), dy(), width(), height(), id(), type()
	{
	}

	Entity::Entity(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
		x(x0), y(y0), dx(dx0), dy(dy0), width(width0), height(height0)
	{
	}

	float_t Entity::calcDistance(const Pointf& pos) const
	{
		return CalcDistance(getPosition(), pos);
	}

	// 点与前进方向的垂足
	std::pair<Pointf, float_t> Entity::calcFootPoint(const Pointf& pos) const
	{
		if (isHolding())
			return std::make_pair(Pointf(x, y), 0.0f);

		// 到达垂足的帧数
		float_t frame = ((pos.x - x) * dx + (pos.y - y) * dy) / (dx * dx + dy * dy);
		return std::make_pair(Pointf(x + dx * frame, y + dy * frame), frame);
	}

	float_t Entity::calcAngle(const Pointf& pos) const
	{
		if (isHolding() || getPosition() == pos)
			return -1.0f;

		return CalcAngle(getPosition(), getNextPos(), pos);
	}

	Direction Entity::calcDirection() const
	{
		if (isHolding())
			return DIR_HOLD;

		// 前进方向与X轴正方向的角度
		float_t angle = CalcAngle(getPosition(), getNextPos(), Pointf(x + 100.0f, y));
		if (dy > 0.0f)	// 转换成360度
			angle = 360.0f - angle;

		// 22.5 = 360 / 8 / 2
		int_t sector = static_cast<int_t>(angle / 22.5f);
		assert(sector >= 0 && sector <= 16);
		return SECTOR_TO_DIR[sector];
	}

	Direction Entity::calcDirection(const Pointf& pos) const
	{
		if (getPosition() == pos)
			return DIR_HOLD;

		// pos与X轴正方向的角度
		float_t angle = CalcAngle(getPosition(), pos, Pointf(x + 100.0f, y));
		if (pos.y > y)	// 转换成360度
			angle = 360.0f - angle;

		// 22.5 = 360 / 8 / 2
		int_t sector = static_cast<int_t>(angle / 22.5f);
		assert(sector >= 0 && sector <= 16);
		return SECTOR_TO_DIR[sector];
	}

	void Entity::advance(float_t frame)
	{
		x += (dx * frame);
		y += (dy * frame);
	}

	bool Entity::collide(const Entity& other) const
	{
		return std::abs(x - other.x) < (width + other.width) / 2.0f
			&& std::abs(y - other.y) < (height + other.height) / 2.0f;
	}

	std::pair<bool, float_t> Entity::willCollideWith(const Entity& other) const
	{
		std::pair<Pointf, float_t> footPoint = calcFootPoint(other.getPosition());
		Entity temp = *this;
		temp.setPosition(footPoint.first);
		if (temp.collide(other))
			return std::make_pair(true, footPoint.second);
		else
			return std::make_pair(false, 0.0f);
	}

	bool Entity::isHighSpeed() const
	{
		return dx > 4.5f || dy > 4.5f;
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
		return std::abs(dx) <= std::numeric_limits<float_t>::epsilon()
			&& std::abs(dy) <= std::numeric_limits<float_t>::epsilon();
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

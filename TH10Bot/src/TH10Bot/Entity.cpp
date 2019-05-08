#include "TH10Bot/Common.h"
#include "TH10Bot/Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace th
{
	// 与X轴正方向的角度扇区转换成移动方向
	const Direction Entity::SECTOR_TO_DIR[17] =
	{
		DIR_RIGHT,		// 0
		DIR_UPRIGHT,
		DIR_UPRIGHT,
		DIR_UP,
		DIR_UP,
		DIR_UPLEFT,
		DIR_UPLEFT,
		DIR_LEFT,
		DIR_LEFT,
		DIR_DOWNLEFT,
		DIR_DOWNLEFT,
		DIR_DOWN,
		DIR_DOWN,
		DIR_DOWNRIGHT,
		DIR_DOWNRIGHT,
		DIR_RIGHT,
		DIR_RIGHT		// 360
	};

	float_t Entity::GetDist(const Pointf& A, const Pointf& B)
	{
		float_t dx = A.x - B.x;
		float_t dy = A.y - B.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	// 余弦定理
	float_t Entity::GetAngle(const Pointf& A, const Pointf& B, const Pointf& C)
	{
		float_t AB = GetDist(A, B);
		float_t AC = GetDist(A, C);
		float_t BC = GetDist(B, C);
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

	float_t Entity::getDist(const Pointf& pos) const
	{
		return GetDist(getPos(), pos);
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

		return GetAngle(getPos(), getNextPos(), pos);
	}

	Direction Entity::getDir() const
	{
		if (isHolded())
			return DIR_HOLD;

		// 前进方向与X轴正方向的角度
		float_t angle = GetAngle(getPos(), getNextPos(), Pointf(x + 100.0f, y));
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
		float_t angle = GetAngle(getPos(), pos, Pointf(x + 100.0f, y));
		if (pos.y > y)	// 转换成360度
			angle = 360.0f - angle;

		// 22.5 = 360 / 8 / 2
		int_t sector = static_cast<int_t>(angle / 22.5f);
		assert(sector >= 0 && sector <= 16);
		return SECTOR_TO_DIR[sector];
	}

	Entity Entity::advance(const Pointf& pos) const
	{
		Entity ret = *this;
		ret.x = pos.x;
		ret.y = pos.y;
		return ret;
	}

	Entity Entity::advance(float_t frame) const
	{
		Entity ret = *this;
		ret.x += (dx * frame);
		ret.y += (dy * frame);
		return ret;
	}

	bool Entity::collide(const Entity& other) const
	{
		return std::abs(x - other.x) < (width + other.width) / 2.0f
			&& std::abs(y - other.y) < (height + other.height) / 2.0f;
	}

	std::pair<bool, float_t> Entity::willCollideWith(const Entity& other) const
	{
		FootPoint footPoint = getFootPoint(other.getPos());
		Entity temp = advance(Pointf(footPoint.x, footPoint.y));
		if (temp.collide(other))
			return std::make_pair(true, footPoint.frame);
		else
			return std::make_pair(false, 0.0f);
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

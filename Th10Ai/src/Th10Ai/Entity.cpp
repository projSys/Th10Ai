#include "Th10Ai/Entity.h"

#include <cassert>

namespace th
{
	// 与X轴正方向的角度扇区转换成移动方向
	const DIR Entity::SECTOR_TO_DIR[17] =
	{
		DIR::RIGHT,		// 0
		DIR::RIGHTUP,
		DIR::RIGHTUP,
		DIR::UP,
		DIR::UP,
		DIR::LEFTUP,
		DIR::LEFTUP,
		DIR::LEFT,
		DIR::LEFT,
		DIR::LEFTDOWN,
		DIR::LEFTDOWN,
		DIR::DOWN,
		DIR::DOWN,
		DIR::RIGHTDOWN,
		DIR::RIGHTDOWN,
		DIR::RIGHT,
		DIR::RIGHT		// 360
	};

	Entity::Entity() :
		x(),
		y(),
		dx(),
		dy(),
		width(),
		height()
	{
	}

	Entity::Entity(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
		x(x0), y(y0), dx(dx0), dy(dy0), width(width0), height(height0)
	{
	}

	float_t Entity::calcDistance(const Pointf& pos) const
	{
		return Utils::CalcDistance(getPosition(), pos);
	}

	float_t Entity::calcAngle(const Pointf& pos) const
	{
		return Utils::CalcAngle(getPosition(), getNextPos(), pos);
	}

	FootPoint Entity::calcFootPoint(const Pointf& pos) const
	{
		return Utils::CalcFootPoint(getPosition(), getNextPos(), pos);
	}

	DIR Entity::calcDirection() const
	{
		if (isHolding())
			return DIR::HOLD;

		// 前进方向与X轴正方向的角度
		float_t angle = Utils::CalcAngle(getPosition(), getNextPos(), Pointf(x + 100.0f, y));
		if (dy > 0.0f)	// 转换成360度
			angle = 360.0f - angle;

		// 22.5 = 360 / 8 / 2
		int_t sector = static_cast<int_t>(angle / 22.5f);
		assert(sector >= 0 && sector <= 16);
		return SECTOR_TO_DIR[sector];
	}

	DIR Entity::calcDirection(const Pointf& pos) const
	{
		if (getPosition() == pos)
			return DIR::HOLD;

		// pos与X轴正方向的角度
		float_t angle = Utils::CalcAngle(getPosition(), pos, Pointf(x + 100.0f, y));
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

	// https://thwiki.cc/%E6%B8%B8%E6%88%8F%E6%94%BB%E7%95%A5/STG%E5%88%A4%E5%AE%9A%E6%95%B0%E6%8D%AE
	bool Entity::collide(const Entity& other) const
	{
		return std::abs(x - other.x) < (width + other.width) / 2.0f
			&& std::abs(y - other.y) < (height + other.height) / 2.0f;
	}

	// 跨帧轨迹碰撞检测
	bool Entity::collide(const Entity& other, float_t frame) const
	{
		if (isHighSpeedWith(other))
		{
			// 计算到当前帧与前一帧之间线段的最近点
			Entity temp = *this;
			temp.advance(frame - 1.0);
			Pointf prevPos = temp.getPosition();	// 以前一帧坐标作为基准
			temp.advance(1.0);
			Pointf curPos = temp.getPosition();
			FootPoint footPoint = Utils::CalcFootPoint(prevPos, curPos, other.getPosition());
			if (footPoint.k > 1.0)	// 还未到达当前帧
				footPoint.k = 1.0;
			if (footPoint.k < 0.0)	// 已经远离一帧以上
				footPoint.k = 1.0;
			temp.setPosition(prevPos);
			temp.advance(footPoint.k);
			return temp.collide(other);
		}
		else
		{
			Entity temp = *this;
			temp.advance(frame);
			return temp.collide(other);
		}
	}

	std::pair<bool, float_t> Entity::willCollideWith(const Entity& other) const
	{
		FootPoint footPoint = calcFootPoint(other.getPosition());
		if (collide(other, footPoint.k))
			return std::make_pair(true, footPoint.k);
		else
			return std::make_pair(false, 0.0f);
	}

	bool Entity::isHighSpeedWith(const Entity& other) const
	{
		return dx > other.width || dy > other.height;
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

	Pointf Entity::getLeftTop() const
	{
		return Pointf(x - width / 2.0f, y - height / 2.0f);
	}

	Pointf Entity::getRightTop() const
	{
		return Pointf(x + width / 2.0f, y - height / 2.0f);
	}

	Pointf Entity::getLeftBottom() const
	{
		return Pointf(x - width / 2.0f, y + height / 2.0f);
	}

	Pointf Entity::getRightBottom() const
	{
		return Pointf(x + width / 2.0f, y + height / 2.0f);
	}

	bool Entity::isHolding() const
	{
		return TypeTraits<float_t>::IsEqual(dx, 0.0f)
			&& TypeTraits<float_t>::IsEqual(dy, 0.0f);
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
		return Rectf(getLeftTop(), getSize());
	}
}

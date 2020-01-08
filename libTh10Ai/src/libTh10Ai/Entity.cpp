#include "libTh10Ai/Common.h"
#include "libTh10Ai/Entity.h"

#include <cassert>
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
		if (TypeTraits<float_t>::Equals(AB, 0.0f)
			|| TypeTraits<float_t>::Equals(AC, 0.0f))
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

	// 首先，求一系数k：设直线的起点和终点分别为A（x1， y1）、B（x2， y2），直线外一点为C（x0， y0），垂足为D；并设 k = |AD| / |AB|。
	// 则 k * AB = AD = AC + CD，又 AB * CD = 0；所以 k * AB * AB = AC * AB，故 k = AC * AB / （AB * AB）。
	// 带入坐标，即得 k = ((x0 - x1) * (x2 - x1) + (y0 - y1) * (y2 - y1)) / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	// 则 x = x1 + k * (x2 - x1); y = y1 + k * (y2 - y1);
	FootPoint Entity::CalcFootPoint(const Pointf& A, const Pointf& B, const Pointf& C)
	{
		FootPoint footPoint = {};

		float_t dxBA = B.x - A.x;
		float_t dyBA = B.y - A.y;
		if (TypeTraits<float_t>::Equals(dxBA, 0.0f)
			&& TypeTraits<float_t>::Equals(dyBA, 0.0f))
		{
			footPoint.pos = A;
			return footPoint;
		}

		float_t dxCA = C.x - A.x;
		float_t dyCA = C.y - A.y;
		footPoint.k = (dxCA * dxBA + dyCA * dyBA) / (dxBA * dxBA + dyBA * dyBA);
		footPoint.pos.x = A.x + footPoint.k * dxBA;
		footPoint.pos.y = A.y + footPoint.k * dyBA;
		return footPoint;
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

	float_t Entity::calcAngle(const Pointf& pos) const
	{
		return CalcAngle(getPosition(), getNextPos(), pos);
	}

	FootPoint Entity::calcFootPoint(const Pointf& pos) const
	{
		return CalcFootPoint(getPosition(), getNextPos(), pos);
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

	Pointf Entity::advance(float_t frame)
	{
		x += (dx * frame);
		y += (dy * frame);
		return Pointf(x, y);
	}

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
			Entity temp = *this;
			//temp.advance(frame);
			//return temp.collide(other);
			Pointf A = temp.advance(frame - 1.0);
			Pointf B = temp.advance(1.0);
			FootPoint footPoint = CalcFootPoint(A, B, other.getPosition());
			if (footPoint.k < 0.0)
				footPoint.k = 0.0;
			if (footPoint.k > 1.0)
				footPoint.k = 1.0;
			temp.setPosition(A);
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
		return TypeTraits<float_t>::Equals(dx, 0.0f)
			&& TypeTraits<float_t>::Equals(dy, 0.0f);
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

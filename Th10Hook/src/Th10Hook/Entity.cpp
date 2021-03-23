#include "Th10Hook/Entity.h"

namespace th
{
	// 首先，求一系数k：设直线的起点和终点分别为A（x1， y1）、B（x2， y2），直线外一点为C（x0， y0），垂足为D；并设 k = |AD| / |AB|。
	// 则 k * AB = AD = AC + CD，又 AB * CD = 0；所以 k * AB * AB = AC * AB，故 k = AC * AB / （AB * AB）。
	// 带入坐标，即得 k = ((x0 - x1) * (x2 - x1) + (y0 - y1) * (y2 - y1)) / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	// 则 x = x1 + k * (x2 - x1); y = y1 + k * (y2 - y1);
	FootPoint CalcFootPoint(const vec2& A, const vec2& B, const vec2& C)
	{
		FootPoint footPoint = {};

		float_t dxBA = B.x - A.x;
		float_t dyBA = B.y - A.y;
		if (TypeTraits<float_t>::Equal(dxBA, 0)
			&& TypeTraits<float_t>::Equal(dyBA, 0))
		{
			footPoint.k = 0;
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

	Entity::Entity()
	{
	}

	Entity::Entity(const vec2& pos0, const vec2& delta0, const vec2& size0) :
		pos(pos0), delta(delta0), size(size0)
	{
	}

	float_t Entity::distance(const Entity& other) const
	{
		return (pos - other.pos).length();
	}

	FootPoint Entity::calcFootPoint(const Entity& other) const
	{
		return CalcFootPoint(pos, getNextPos(), other.pos);
	}

	void Entity::advance(float_t frame)
	{
		pos += (delta * frame);
	}

	// https://thwiki.cc/%E6%B8%B8%E6%88%8F%E6%94%BB%E7%95%A5/STG%E5%88%A4%E5%AE%9A%E6%95%B0%E6%8D%AE
	bool Entity::collide(const Entity& other) const
	{
		return std::abs(pos.x - other.pos.x) < (size.x + other.size.x) / 2
			&& std::abs(pos.y - other.pos.y) < (size.y + other.size.y) / 2;
	}

	bool Entity::collide(const Entity& other, float_t epsilon) const
	{
		return std::abs(pos.x - other.pos.x) < (size.x + other.size.x) / 2 + epsilon
			&& std::abs(pos.y - other.pos.y) < (size.y + other.size.y) / 2 + epsilon;
	}

	bool Entity::collide2(const Entity& other) const
	{
		// 计算到当前帧与前一帧之间线段的最近点
		Entity prev = *this;
		prev.advance(-1);
		FootPoint footPoint = CalcFootPoint(prev.pos, pos, other.pos);
		if (footPoint.k > 1)	// 还未到达当前帧
			footPoint.k = 1;
		if (footPoint.k < 0)	// 已经远离一帧以上
			footPoint.k = 0;
		prev.advance(footPoint.k);
		return prev.collide(other);
	}

	// 跨帧轨迹碰撞检测
	bool Entity::collide3(const Entity& other, float_t frame) const
	{
		if (isHighSpeedWith(other))
		{
			// 计算到当前帧与前一帧之间线段的最近点
			Entity temp = *this;
			temp.advance(frame - 1);
			vec2 prevPos = temp.pos;	// 以前一帧坐标作为基准
			temp.advance(1);
			vec2 curPos = temp.pos;
			FootPoint footPoint = CalcFootPoint(prevPos, curPos, other.pos);
			if (footPoint.k > 1)	// 还未到达当前帧
				footPoint.k = 1;
			if (footPoint.k < 0)	// 已经远离一帧以上
				footPoint.k = 0;
			temp.pos = prevPos;
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
		FootPoint footPoint = calcFootPoint(other);
		Entity temp = *this;
		temp.advance(footPoint.k);
		if (temp.collide(other))
			return std::make_pair(true, footPoint.k);
		else
			return std::make_pair(false, float_t());
	}

	bool Entity::isHighSpeedWith(const Entity& other) const
	{
		return delta.x > other.delta.x || delta.y > other.delta.y;
	}

	vec2 Entity::getLeftTop() const
	{
		return vec2(pos.x - size.x / 2, pos.y - size.y / 2);
	}

	vec2 Entity::getRightTop() const
	{
		return vec2(pos.x + size.x / 2, pos.y - size.y / 2);
	}

	vec2 Entity::getLeftBottom() const
	{
		return vec2(pos.x - size.x / 2, pos.y + size.y / 2);
	}

	vec2 Entity::getRightBottom() const
	{
		return vec2(pos.x + size.x / 2, pos.y + size.y / 2);
	}

	bool Entity::isHolding() const
	{
		return TypeTraits<float_t>::Equal(delta.x, 0)
			&& TypeTraits<float_t>::Equal(delta.y, 0);
	}

	vec2 Entity::getNextPos() const
	{
		return pos + delta;
	}
}

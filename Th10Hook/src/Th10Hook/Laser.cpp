#include "Th10Hook/Laser.h"

#include <math.h>
#include <cmath>

namespace th
{
	Laser::Laser() :
		arc()
	{
	}

	Laser::Laser(const vec2& pos0, const vec2& delta0, const vec2& size0, float_t arc0) :
		pos(pos0), delta(delta0), size(size0), arc(arc0)
	{
	}

	void Laser::rotate()
	{
		// emmm...你说这个谁懂啊？
		float_t radian = arc - static_cast<float_t>(M_PI) * 5 / 2;

		leftTop = pos + (getLeftTop() - pos).rotate(radian);
		rightTop = pos + (getRightTop() - pos).rotate(radian);
		leftBottom = pos + (getLeftBottom() - pos).rotate(radian);
		rightBottom = pos + (getRightBottom() - pos).rotate(radian);
		axisX = vec2(1, 0).rotate(radian);
		axisY = vec2(0, 1).rotate(radian);
	}

	bool Laser::overlap(const vec2& lt2, const vec2& rt2, const vec2& lb2, const vec2& rb2,
		const vec2& axis) const
	{
		// a·b = |a||b|cosθ
		// |b| = 1
		// a·b = |a|cosθ
		// |a|cosθ即a在b上的投影
		float_t proj1 = leftTop.dot(axis);
		float_t proj2 = rightTop.dot(axis);
		float_t proj3 = leftBottom.dot(axis);
		float_t proj4 = rightBottom.dot(axis);

		float_t proj5 = lt2.dot(axis);
		float_t proj6 = rt2.dot(axis);
		float_t proj7 = lb2.dot(axis);
		float_t proj8 = rb2.dot(axis);

		float_t min1 = std::min(std::min(proj1, proj2), std::min(proj3, proj4));
		float_t max1 = std::max(std::max(proj1, proj2), std::max(proj3, proj4));

		float_t min2 = std::min(std::min(proj5, proj6), std::min(proj7, proj8));
		float_t max2 = std::max(std::max(proj5, proj6), std::max(proj7, proj8));

		return max1 > min2 && max2 > min1;
		//return !(max1 < min2 || max2 < min1);
	}

	bool Laser::collide(const Entity& other) const
	{
		vec2 leftTop2 = other.getLeftTop();
		vec2 rightTop2 = other.getRightTop();
		vec2 leftBottom2 = other.getLeftBottom();
		vec2 rightBottom2 = other.getRightBottom();
		vec2 axisX2(1, 0);
		vec2 axisY2(0, 1);

		if (!overlap(leftTop2, rightTop2, leftBottom2, rightBottom2,
			axisX))
			return false;

		if (!overlap(leftTop2, rightTop2, leftBottom2, rightBottom2,
			axisY))
			return false;

		if (!overlap(leftTop2, rightTop2, leftBottom2, rightBottom2,
			axisX2))
			return false;

		if (!overlap(leftTop2, rightTop2, leftBottom2, rightBottom2,
			axisY2))
			return false;

		return true;
	}

	// 首先，求一系数k：设直线的起点和终点分别为A（x1， y1）、B（x2， y2），直线外一点为C（x0， y0），垂足为D；并设 k = |AD| / |AB|。
	// 则 k * AB = AD = AC + CD，又 AB * CD = 0；所以 k * AB * AB = AC * AB，故 k = AC * AB / （AB * AB）。
	// 带入坐标，即得 k = ((x0 - x1) * (x2 - x1) + (y0 - y1) * (y2 - y1)) / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	// 则 x = x1 + k * (x2 - x1); y = y1 + k * (y2 - y1);
	FootPoint CalcFootPoint1(const vec2& A, const vec2& B, const vec2& C)
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

	FootPoint Laser::calcFootPoint(const Entity& other) const
	{
		return CalcFootPoint1(pos, getNextPos(), other.pos);
	}

	//todo 错的
	std::pair<bool, float_t> Laser::willCollideWith(const Entity& other) const
	{
		FootPoint footPoint = calcFootPoint(other);
		Laser temp = *this;
		temp.advance(footPoint.k);
		if (temp.collide(other))
			return std::make_pair(true, footPoint.k);
		else
			return std::make_pair(false, float_t());
	}

	vec2 Laser::getCenter() const
	{
		return vec2(pos.x, pos.y + size.y / 2);
	}

	vec2 Laser::getLeftTop() const
	{
		return vec2(pos.x - size.x / 2, pos.y);
	}

	vec2 Laser::getRightTop() const
	{
		return vec2(pos.x + size.x / 2, pos.y);
	}

	vec2 Laser::getLeftBottom() const
	{
		return vec2(pos.x - size.x / 2, pos.y + size.y);
	}

	vec2 Laser::getRightBottom() const
	{
		return vec2(pos.x + size.x / 2, pos.y + size.y);
	}

	vec2 Laser::getNextPos() const
	{
		return pos + delta;
	}

	void Laser::advance(float_t frame)
	{
		pos += (delta * frame);
	}
}

#include "Th10Hook/Laser.h"

#include <math.h>
#include <cmath>

namespace th
{
	Laser::Laser() :
		arc()
	{
	}

	bool Laser::collide(const Entity& other) const
	{
		LaserBox laserBox(*this);
		if (!laserBox.collide(other))
			return false;

		EntityBox entityBox(other, *this);
		if (!entityBox.collide(*this))
			return false;

		return true;
	}

	// 错的
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



	// 平面中，一个点(px, py)绕任意点(cx, cy)逆时针旋转a度后的坐标
	// x = (px - cx) * cos(a) - (py - cy) * sin(a) + cx;
	// y = (px - cx) * sin(a) + (py - cy) * cos(a) + cy;
	// 平面中，一个点(px, py)绕任意点(cx, cy)顺时针旋转a度后的坐标
	// x = (px - cx) * cos(-a) - (py - cy) * sin(-a) + cx;
	// y = (px - cx) * sin(-a) + (py - cy) * cos(-a) + cy;
	vec2 RotatePoint(const vec2& P, const vec2& C, float_t sinC, float_t cosC)
	{
		float_t dx = P.x - C.x;
		float_t dy = P.y - C.y;
		float_t x = dx * cosC - dy * sinC + C.x;
		float_t y = dx * sinC + dy * cosC + C.y;
		return vec2(x, y);
	}

	bool Overlap(float_t minA, float_t maxA, float_t minB, float_t maxB)
	{
		return maxA > minB && maxB > minA;
		//return !(maxA < minB || maxB < minA);
	}

	LaserBox::LaserBox(const Laser& laser)
	{
		vec2 C = laser.pos;
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5 / 2;
		float_t sinC = std::sin(radianC);
		float_t cosC = std::cos(radianC);

		//leftTop = (laser.getLeftTop() - C).rotate(radianC) + C;
		leftTop = RotatePoint(laser.getLeftTop(), C, sinC, cosC);
		rightTop = RotatePoint(laser.getRightTop(), C, sinC, cosC);
		leftBottom = RotatePoint(laser.getLeftBottom(), C, sinC, cosC);
		rightBottom = RotatePoint(laser.getRightBottom(), C, sinC, cosC);
	}

	// 分离轴定理
	bool LaserBox::collide(const Entity& other) const
	{
		// 投影到X轴
		float_t minX = std::min(std::min(leftTop.x, rightTop.x), std::min(leftBottom.x, rightBottom.x));
		float_t maxX = std::max(std::max(leftTop.x, rightTop.x), std::max(leftBottom.x, rightBottom.x));
		// 检测2条线段是否重叠
		if (!Overlap(minX, maxX, other.pos.x - other.size.x / 2, other.pos.x + other.size.x / 2))
			return false;

		// 投影到Y轴
		float_t minY = std::min(std::min(leftTop.y, rightTop.y), std::min(leftBottom.y, rightBottom.y));
		float_t maxY = std::max(std::max(leftTop.y, rightTop.y), std::max(leftBottom.y, rightBottom.y));
		// 检测2条线段是否重叠
		if (!Overlap(minY, maxY, other.pos.y - other.size.y / 2, other.pos.y + other.size.y / 2))
			return false;

		return true;
	}

	// 错的
	// 反向旋转画布，即激光不变，反向旋转自机坐标
	EntityBox::EntityBox(const Entity& entity, const Laser& laser)
	{
		vec2 C = laser.pos;
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5 / 2;
		float_t sinC = std::sin(-radianC);
		float_t cosC = std::cos(-radianC);

		leftTop = RotatePoint(entity.getLeftTop(), C, sinC, cosC);
		rightTop = RotatePoint(entity.getRightTop(), C, sinC, cosC);
		leftBottom = RotatePoint(entity.getLeftBottom(), C, sinC, cosC);
		rightBottom = RotatePoint(entity.getRightBottom(), C, sinC, cosC);
	}

	// 分离轴定理
	bool EntityBox::collide(const Laser& other) const
	{
		// 投影到X轴
		float_t minX = std::min(std::min(leftTop.x, rightTop.x), std::min(leftBottom.x, rightBottom.x));
		float_t maxX = std::max(std::max(leftTop.x, rightTop.x), std::max(leftBottom.x, rightBottom.x));
		// 检测2条线段是否重叠
		if (!Overlap(minX, maxX, other.pos.x - other.size.x / 2, other.pos.x + other.size.x / 2))
			return false;

		// 投影到Y轴
		float_t minY = std::min(std::min(leftTop.y, rightTop.y), std::min(leftBottom.y, rightBottom.y));
		float_t maxY = std::max(std::max(leftTop.y, rightTop.y), std::max(leftBottom.y, rightBottom.y));
		// 检测2条线段是否重叠
		if (!Overlap(minY, maxY, other.pos.y, other.pos.y + other.size.y))
			return false;

		return true;
	}
}

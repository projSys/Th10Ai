#include "Th10Hook/Laser.h"

#include <math.h>
#include <cmath>

namespace th
{
	Laser::Laser() :
		arc()
	{
	}

	//Laser::Laser(const vec2& pos0, const vec2& delta0, const vec2& size0, float_t arc0) :
	//	pos(pos0), delta(delta0), size(size0), arc(arc0)
	//{
	//}

	bool Projection(const vec2& l1, const vec2& l2, const vec2& l3, const vec2& l4,
		const vec2& e1, const vec2& e2, const vec2& e3, const vec2& e4,
		const vec2& axis)
	{
		// a·b = |a||b|cosθ
		// |b| = 1
		// a·b = |a|cosθ
		float_t lp1 = l1.dot(axis);
		float_t lp2 = l2.dot(axis);
		float_t lp3 = l3.dot(axis);
		float_t lp4 = l4.dot(axis);

		float_t ep1 = e1.dot(axis);
		float_t ep2 = e2.dot(axis);
		float_t ep3 = e3.dot(axis);
		float_t ep4 = e4.dot(axis);

		float_t min1 = std::min(std::min(lp1, lp2), std::min(lp3, lp4));
		float_t max1 = std::max(std::max(lp1, lp2), std::max(lp3, lp4));

		float_t min2 = std::min(std::min(ep1, ep2), std::min(ep3, ep4));
		float_t max2 = std::max(std::max(ep1, ep2), std::max(ep3, ep4));

		return max1 > min2 && max2 > min1;
		//return !(max1 < min2 || max2 < min1);
	}

	bool Laser::collide(const Entity& other) const
	{
		//// emmm...你说这个谁懂啊？
		//float_t radian = arc - static_cast<float_t>(M_PI) * 5 / 2;

		//vec2 center = getCenter();
		//vec2 leftTop = center + (getLeftTop() - center).rotate(radian);
		//vec2 rightTop = center + (getRightTop() - center).rotate(radian);
		//vec2 leftBottom = center + (getLeftBottom() - center).rotate(radian);
		//vec2 rightBottom = center + (getRightBottom() - center).rotate(radian);

		//vec2 axisX1(1, 0);	// 未旋转的对称轴，单位向量
		//vec2 axisY1(0, 1);
		//vec2 axisX2 = axisX1.rotate(radian);	// 已旋转的对称轴
		//vec2 axisY2 = axisY1.rotate(radian);

		//if (!Projection(leftTop, rightTop, leftBottom, rightBottom,
		//	other.getLeftTop(), other.getRightTop(), other.getLeftBottom(), other.getRightBottom(),
		//	axisX1))
		//	return false;

		//if (!Projection(leftTop, rightTop, leftBottom, rightBottom,
		//	other.getLeftTop(), other.getRightTop(), other.getLeftBottom(), other.getRightBottom(),
		//	axisY1))
		//	return false;

		//if (!Projection(leftTop, rightTop, leftBottom, rightBottom,
		//	other.getLeftTop(), other.getRightTop(), other.getLeftBottom(), other.getRightBottom(),
		//	axisX2))
		//	return false;

		//if (!Projection(leftTop, rightTop, leftBottom, rightBottom,
		//	other.getLeftTop(), other.getRightTop(), other.getLeftBottom(), other.getRightBottom(),
		//	axisY2))
		//	return false;

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

	bool Overlap(float_t min1, float_t max1, float_t min2, float_t max2)
	{
		return max1 > min2 && max2 > min1;
		//return !(max1 < min2 || max2 < min1);
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
		if (!Overlap(minX, maxX, other.getLeftTop().x, other.getRightTop().x))
			return false;

		// 投影到Y轴
		float_t minY = std::min(std::min(leftTop.y, rightTop.y), std::min(leftBottom.y, rightBottom.y));
		float_t maxY = std::max(std::max(leftTop.y, rightTop.y), std::max(leftBottom.y, rightBottom.y));
		// 检测2条线段是否重叠
		if (!Overlap(minY, maxY, other.getLeftTop().y, other.getLeftBottom().y))
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

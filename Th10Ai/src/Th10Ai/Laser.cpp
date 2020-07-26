#include "Th10Ai/Common.h"
#include "Th10Ai/Laser.h"

#define _USE_MATH_DEFINES
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

	// 大概应该没有问题吧？
	pair<bool, float_t> Laser::willCollideWith(const Entity& other) const
	{
		FootPoint footPoint = calcFootPoint(other.getPosition());
		Laser temp = *this;
		temp.setPosition(footPoint.pos);
		if (temp.collide(other))
			return make_pair(true, footPoint.k);
		else
			return make_pair(false, 0.0f);
	}

	Pointf Laser::getLeftTop() const
	{
		return Pointf(x - width / 2.0f, y);
	}

	Pointf Laser::getRightTop() const
	{
		return Pointf(x + width / 2.0f, y);
	}

	Pointf Laser::getLeftBottom() const
	{
		return Pointf(x - width / 2.0f, y + height);
	}

	Pointf Laser::getRightBottom() const
	{
		return Pointf(x + width / 2.0f, y + height);
	}



	// 平面中，一个点(x, y)绕任意点(cx, cy)顺时针旋转a度后的坐标
	// xx = (x - cx) * cos(-a) - (y - cy) * sin(-a) + cx;
	// yy = (x - cx) * sin(-a) + (y - cy) * cos(-a) + cy;
	// 平面中，一个点(x, y)绕任意点(cx, cy)逆时针旋转a度后的坐标
	// xx = (x - cx) * cos(a) - (y - cy) * sin(a) + cx;
	// yy = (x - cx) * sin(a) + (y - cy) * cos(a) + cy;
	Pointf SatBox::Rotate(const Pointf& P, const Pointf& C, float_t radianC)
	{
		float_t dx = P.x - C.x;
		float_t dy = P.y - C.y;
		float_t sinC = std::sin(radianC);
		float_t cosC = std::cos(radianC);
		return Pointf(dx * cosC - dy * sinC + C.x, dx * sinC + dy * cosC + C.y);
	}

	bool SatBox::Collide(float_t p1, float_t s1, float_t p2, float_t s2)
	{
		return std::abs(p1 - p2) < (s1 + s2) / 2.0f;
	}

	LaserBox::LaserBox(const Laser& laser)
	{
		Pointf C = laser.getPosition();
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		leftTop = Rotate(laser.getLeftTop(), C, radianC);
		rightTop = Rotate(laser.getRightTop(), C, radianC);
		leftBottom = Rotate(laser.getLeftBottom(), C, radianC);
		rightBottom = Rotate(laser.getRightBottom(), C, radianC);
	}

	// 分离轴定理
	bool LaserBox::collide(const Entity& other) const
	{
		// 投影到X轴
		float_t minX = std::min(std::min(leftTop.x, rightTop.x), std::min(leftBottom.x, rightBottom.x));
		float_t maxX = std::max(std::max(leftTop.x, rightTop.x), std::max(leftBottom.x, rightBottom.x));
		// 检测2条线段是否重叠
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, other.x, other.width))
			return false;

		// 投影到Y轴
		float_t minY = std::min(std::min(leftTop.y, rightTop.y), std::min(leftBottom.y, rightBottom.y));
		float_t maxY = std::max(std::max(leftTop.y, rightTop.y), std::max(leftBottom.y, rightBottom.y));
		// 检测2条线段是否重叠
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, other.y, other.height))
			return false;

		return true;
	}

	// 反向旋转画布，即把激光转回来，再反向旋转自机坐标
	EntityBox::EntityBox(const Entity& entity, const Laser& laser)
	{
		Pointf C = laser.getPosition();
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		leftTop = Rotate(entity.getLeftTop(), C, -radianC);
		rightTop = Rotate(entity.getRightTop(), C, -radianC);
		leftBottom = Rotate(entity.getLeftBottom(), C, -radianC);
		rightBottom = Rotate(entity.getRightBottom(), C, -radianC);
	}

	// 分离轴定理
	bool EntityBox::collide(const Laser& other) const
	{
		// 投影到X轴
		float_t minX = std::min(std::min(leftTop.x, rightTop.x), std::min(leftBottom.x, rightBottom.x));
		float_t maxX = std::max(std::max(leftTop.x, rightTop.x), std::max(leftBottom.x, rightBottom.x));
		// 检测2条线段是否重叠
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, other.x, other.width))
			return false;

		// 投影到Y轴
		float_t minY = std::min(std::min(leftTop.y, rightTop.y), std::min(leftBottom.y, rightBottom.y));
		float_t maxY = std::max(std::max(leftTop.y, rightTop.y), std::max(leftBottom.y, rightBottom.y));
		// 检测2条线段是否重叠
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, other.y + other.height / 2.0f, other.height))
			return false;

		return true;
	}
}

#include "TH10Bot/Common.h"
#include "TH10Bot/Laser.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "TH10Bot/Player.h"

namespace th
{
	Laser Laser::advance(const Pointf& pos) const
	{
		Laser ret = *this;
		ret.x = pos.x;
		ret.y = pos.y;
		return ret;
	}

	Laser Laser::advance(float_t frame) const
	{
		Laser ret = *this;
		ret.x += (dx * frame);
		ret.y += (dy * frame);
		return ret;
	}

	bool Laser::collide(const Player& player) const
	{
		LaserBox laserBox(*this);
		if (!laserBox.collide(player))
			return false;

		PlayerBox playerBox(player, *this);
		if (!playerBox.collide(*this))
			return false;

		return true;
	}

	Pointf Laser::getTopLeft() const
	{
		return Pointf(x - width / 2.0f, y);
	}

	Pointf Laser::getTopRight() const
	{
		return Pointf(x + width / 2.0f, y);
	}

	Pointf Laser::getBottomLeft() const
	{
		return Pointf(x - width / 2.0f, y + height);
	}

	Pointf Laser::getBottomRight() const
	{
		return Pointf(x + width / 2.0f, y + height);
	}



	// 平面中，一个点(x, y)绕任意点(cx, cy)顺时针旋转a度后的坐标
	// xx = (x - cx) * cos(-a) - (y - cy) * sin(-a) + cx;
	// yy = (x - cx) * sin(-a) + (y - cy) * cos(-a) + cy;
	// 平面中，一个点(x, y)绕任意点(cx, cy)逆时针旋转a度后的坐标
	// xx = (x - cx) * cos(a) - (y - cy) * sin(a) + cx;
	// yy = (x - cx) * sin(a) + (y - cy) * cos(a) + cy;
	Pointf SATBox::Rotate(const Pointf& P, const Pointf& C, float_t radianC)
	{
		float_t dx = P.x - C.x;
		float_t dy = P.y - C.y;
		float_t sinC = std::sin(radianC);
		float_t cosC = std::cos(radianC);
		return Pointf(dx * cosC - dy * sinC + C.x, dx * sinC + dy * cosC + C.y);
	}

	bool SATBox::Collide(float_t c1, float_t s1, float_t c2, float_t s2)
	{
		return std::abs(c1 - c2) < (s1 + s2) / 2.0f;
	}

	LaserBox::LaserBox(const Laser& laser)
	{
		Pointf C = laser.getPos();
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = Rotate(laser.getTopLeft(), C, radianC);
		topRight = Rotate(laser.getTopRight(), C, radianC);
		bottomLeft = Rotate(laser.getBottomLeft(), C, radianC);
		bottomRight = Rotate(laser.getBottomRight(), C, radianC);
	}

	// 分离轴定理
	bool LaserBox::collide(const Player& player) const
	{
		// 投影到X轴
		float_t minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
		float_t maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
		// 检测2条线段是否重叠
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, player.x, player.width))
			return false;

		// 投影到Y轴
		float_t minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
		float_t maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));
		// 检测2条线段是否重叠
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, player.y, player.height))
			return false;

		return true;
	}

	// 反向旋转画布，即把激光转回来，再反向旋转自机坐标
	PlayerBox::PlayerBox(const Player& player, const Laser& laser)
	{
		Pointf C = laser.getPos();
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = Rotate(player.getTopLeft(), C, -radianC);
		topRight = Rotate(player.getTopRight(), C, -radianC);
		bottomLeft = Rotate(player.getBottomLeft(), C, -radianC);
		bottomRight = Rotate(player.getBottomRight(), C, -radianC);
	}

	// 分离轴定理
	bool PlayerBox::collide(const Laser& laser) const
	{
		// 投影到X轴
		float_t minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
		float_t maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
		// 检测2条线段是否重叠
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, laser.x, laser.width))
			return false;

		// 投影到Y轴
		float_t minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
		float_t maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));
		// 检测2条线段是否重叠
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, laser.y + laser.height / 2.0f, laser.height))
			return false;

		return true;
	}
}

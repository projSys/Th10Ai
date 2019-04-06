#include "TH10Bot/Common.h"
#include "TH10Bot/Entity/Laser.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "TH10Bot/MyMath.h"
#include "TH10Bot/Entity/Player.h"

namespace th
{
	bool Laser::collide(const Player& player) const
	{
		SATBox laserBox(*this);
		if (!laserBox.collide(player))
			return false;

		SATBox playerBox(player, *this);
		if (!playerBox.collide(*this))
			return false;

		return true;
	}

	bool Laser::collide(const Player& player, float_t frame) const
	{
		Laser temp = *this;
		temp.x += temp.dx * frame;
		temp.y += temp.dy * frame;
		return temp.collide(player);
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



	bool SATBox::Collide(float_t p1, float_t s1, float_t p2, float_t s2)
	{
		return std::abs(p1 - p2) < (s1 + s2) / 2.0f;
	}

	SATBox::SATBox(const Laser& laser)
	{
		Pointf C = laser.getPosition();
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = MyMath::Rotate(laser.getTopLeft(), C, radianC);
		topRight = MyMath::Rotate(laser.getTopRight(), C, radianC);
		bottomLeft = MyMath::Rotate(laser.getBottomLeft(), C, radianC);
		bottomRight = MyMath::Rotate(laser.getBottomRight(), C, radianC);
	}

	// 反向旋转画布，即把激光转回来，再反向旋转自机坐标
	SATBox::SATBox(const Player& player, const Laser& laser)
	{
		Pointf C = laser.getPosition();
		// emmm...你说这个谁懂啊？
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = MyMath::Rotate(player.getTopLeft(), C, -radianC);
		topRight = MyMath::Rotate(player.getTopRight(), C, -radianC);
		bottomLeft = MyMath::Rotate(player.getBottomLeft(), C, -radianC);
		bottomRight = MyMath::Rotate(player.getBottomRight(), C, -radianC);
	}

	// 分离轴定理
	bool SATBox::collide(const Player& player) const
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

	// 分离轴定理
	bool SATBox::collide(const Laser& laser) const
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

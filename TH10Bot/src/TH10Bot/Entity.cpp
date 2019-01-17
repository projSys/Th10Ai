#include "TH10Bot/Common.h"
#include "TH10Bot/Entity.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace th
{
	float_t Entity::distance(const Pointf& other) const
	{
		float_t dx = x - other.x;
		float_t dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	float_t Entity::distance(const Entity& other) const
	{
		float_t dx = x - other.x;
		float_t dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	float_t Entity::angle(const Entity& other) const
	{
		//if (std::abs(dx) <= std::numeric_limits<float_t>::epsilon()
		//	&& std::abs(dy) <= std::numeric_limits<float_t>::epsilon())
		//	return 360.0f;
		Pointf nextPos = getNextPos();
		float_t AB = distance(other);
		float_t AC = distance(nextPos);
		float_t BC = other.distance(nextPos);
		// 余弦定理
		float_t cosA = (AB * AB + AC * AC - BC * BC) / (2.0f * AB * AC);
		float_t radian = std::acos(cosA);
		// 角度 = 弧度 * 180 / PI
		return static_cast<float_t>(radian * 180.0f / M_PI);
	}

	Pointf Entity::footPoint(const Entity& other) const
	{
		if (std::abs(dx) <= std::numeric_limits<float_t>::epsilon()
			&& std::abs(dy) <= std::numeric_limits<float_t>::epsilon())
			return Pointf(x, y);
		//Pointf nextPos = getNextPos();
		//float_t u = (other.x - x) * (nextPos.x - x) + (other.y - y) * (nextPos.y - y);
		float_t u = (other.x - x) * dx + (other.y - y) * dy;
		u /= (dx * dx + dy * dy);
		return Pointf(x + u * dx, y + u * dy);
	}

	bool Entity::hitTest(const Entity& other, float_t epsilon) const
	{
		return std::abs(x - other.x) < (width + other.width) / 2.0f + epsilon
			&& std::abs(y - other.y) < (height + other.height) / 2.0f + epsilon;
	}

	Pointf Entity::getCenter() const
	{
		return Pointf(x, y);
	}

	Pointf Entity::getLeftTop() const
	{
		return Pointf(x - width / 2.0f, y - height / 2.0f);
	}

	Pointf Entity::getRightBottom() const
	{
		return Pointf(x + width / 2.0f, y + height / 2.0f);
	}

	Pointf Entity::getNextPos() const
	{
		return Pointf(x + dx, y + dy);
	}



	bool Player::hitTestSAT(const Laser& laser, float_t epsilon) const
	{
		LaserBox laserBox(laser);
		if (!laserBox.hitTestSAT(*this, epsilon))
			return false;

		PlayerBox playerBox(*this, laser);
		if (!playerBox.hitTestSAT(laser, epsilon))
			return false;

		return true;
	}



	Bullet Bullet::advance() const
	{
		return Bullet(x + dx, y + dy, width, height, dx, dy);
	}



	void SATBox::PointRotate(float_t& x, float_t& y, float_t cx, float_t cy, float_t arc)
	{
		// 设平面上一点（x,y），绕另一点（cx,cy）逆时针旋转A角度后的点为（x0,y0）,则：
		float_t x0 = (x - cx) * std::cos(arc) - (y - cy) * std::sin(arc) + cx;
		float_t y0 = (x - cx) * std::sin(arc) + (y - cy) * std::cos(arc) + cy;
		x = x0;
		y = y0;
	}

	bool SATBox::HitTest(float_t c1, float_t d1, float_t c2, float_t d2, float_t epsilon)
	{
		return std::abs(c1 - c2) < (d1 + d2) / 2.0 + epsilon;
	}

	LaserBox::LaserBox(const Laser& laser)
	{
		x1 = laser.x - laser.width / 2.0; y1 = laser.y;
		x2 = laser.x + laser.width / 2.0; y2 = laser.y;
		x3 = laser.x - laser.width / 2.0; y3 = laser.y + laser.height;
		x4 = laser.x + laser.width / 2.0; y4 = laser.y + laser.height;
		// emmm?
		float_t arc = laser.arc - 3.14159265358979323846 * 5.0 / 2.0;
		PointRotate(x1, y1, laser.x, laser.y, arc);
		PointRotate(x2, y2, laser.x, laser.y, arc);
		PointRotate(x3, y3, laser.x, laser.y, arc);
		PointRotate(x4, y4, laser.x, laser.y, arc);
	}

	// 分离轴定理
	bool LaserBox::hitTestSAT(const Player& player, float_t epsilon) const
	{
		float_t xMin = std::min(std::min(x1, x2), std::min(x3, x4));
		float_t xMax = std::max(std::max(x1, x2), std::max(x3, x4));
		if (!HitTest(xMin + (xMax - xMin) / 2.0, xMax - xMin, player.x, player.width, epsilon))
			return false;

		float_t yMin = std::min(std::min(y1, y2), std::min(y3, y4));
		float_t yMax = std::max(std::max(y1, y2), std::max(y3, y4));
		if (!HitTest(yMin + (yMax - yMin) / 2.0, yMax - yMin, player.y, player.height, epsilon))
			return false;

		return true;
	}

	// 反向旋转画布，即把激光转回来，再反向旋转自机坐标
	PlayerBox::PlayerBox(const Player& player, const Laser& laser)
	{
		x1 = player.x - player.width / 2.0; y1 = player.y - player.height / 2.0;
		x2 = player.x + player.width / 2.0; y2 = player.y - player.height / 2.0;
		x3 = player.x - player.width / 2.0; y3 = player.y + player.height / 2.0;
		x4 = player.x + player.width / 2.0; y4 = player.y + player.height / 2.0;
		// emmm?
		float_t arc = laser.arc - 3.14159265358979323846 * 5.0 / 2.0;
		PointRotate(x1, y1, laser.x, laser.y, -arc);
		PointRotate(x2, y2, laser.x, laser.y, -arc);
		PointRotate(x3, y3, laser.x, laser.y, -arc);
		PointRotate(x4, y4, laser.x, laser.y, -arc);
	}

	// 分离轴定理
	bool PlayerBox::hitTestSAT(const Laser& laser, float_t epsilon) const
	{
		float_t xMin = std::min(std::min(x1, x2), std::min(x3, x4));
		float_t xMax = std::max(std::max(x1, x2), std::max(x3, x4));
		if (!HitTest(xMin + (xMax - xMin) / 2.0, xMax - xMin, laser.x, laser.width, epsilon))
			return false;

		float_t yMin = std::min(std::min(y1, y2), std::min(y3, y4));
		float_t yMax = std::max(std::max(y1, y2), std::max(y3, y4));
		if (!HitTest(yMin + (yMax - yMin) / 2.0, yMax - yMin, laser.y + laser.height / 2.0, laser.height, epsilon))
			return false;

		return true;
	}
}

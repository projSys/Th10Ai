#include "TH10Bot/Common.h"
#include "TH10Bot/Entity.h"

namespace th
{
	bool Player::hitTest(const Rect2d& other, double epsilon) const
	{
		return std::abs(x - other.x) < (w + other.w) / 2.0 + epsilon
			&& std::abs(y - other.y) < (h + other.h) / 2.0 + epsilon;
	}

	bool Player::hitTestSAT(const Laser& laser, double epsilon) const
	{
		LaserBox laserBox(laser);
		if (!laserBox.hitTestSAT(*this, epsilon))
			return false;

		PlayerBox playerBox(*this, laser);
		if (!playerBox.hitTestSAT(laser, epsilon))
			return false;

		return true;
	}

	bool Player::hitTest(const std::vector<Enemy>& enemies, double epsilon) const
	{
		for (const Enemy& enemy : enemies)
		{
			if (hitTest(enemy, epsilon))
				return true;
		}

		return false;
	}

	bool Player::hitTest(const std::vector<Bullet>& bullets, double epsilon) const
	{
		for (const Bullet& bullet : bullets)
		{
			if (hitTest(bullet, epsilon))
				return true;
		}

		return false;
	}

	bool Player::hitTestSAT(const std::vector<Laser>& lasers, double epsilon) const
	{
		for (const Laser& laser : lasers)
		{
			if (hitTestSAT(laser, epsilon))
				return true;
		}

		return false;
	}



	Bullet Bullet::advance() const
	{
		return Bullet(x + dx, y + dy, w, h, dx, dy);
	}

	// 余弦定理
	double Bullet::angle(const Player& player) const
	{
		Bullet next = advance();
		double ab = distance(next);
		double ac = distance(player);
		double bc = next.distance(player);
		double cosA = (ab * ab + ac * ac - bc * bc) / (2.0 * ab * ac);
		return std::acos(cosA) * 180.0 / 3.14159265358979323846;
	}



	void SATBox::PointRotate(double& x, double& y, double cx, double cy, double arc)
	{
		// 设平面上一点（x,y），绕另一点（cx,cy）逆时针旋转A角度后的点为（x0,y0）,则：
		double x0 = (x - cx) * std::cos(arc) - (y - cy) * std::sin(arc) + cx;
		double y0 = (x - cx) * std::sin(arc) + (y - cy) * std::cos(arc) + cy;
		x = x0;
		y = y0;
	}

	bool SATBox::HitTest(double c1, double d1, double c2, double d2, double epsilon)
	{
		return std::abs(c1 - c2) < (d1 + d2) / 2.0 + epsilon;
	}

	LaserBox::LaserBox(const Laser& laser)
	{
		x1 = laser.x - laser.w / 2.0; y1 = laser.y;
		x2 = laser.x + laser.w / 2.0; y2 = laser.y;
		x3 = laser.x - laser.w / 2.0; y3 = laser.y + laser.h;
		x4 = laser.x + laser.w / 2.0; y4 = laser.y + laser.h;
		// emmm?
		double arc = laser.arc - 3.14159265358979323846 * 5.0 / 2.0;
		PointRotate(x1, y1, laser.x, laser.y, arc);
		PointRotate(x2, y2, laser.x, laser.y, arc);
		PointRotate(x3, y3, laser.x, laser.y, arc);
		PointRotate(x4, y4, laser.x, laser.y, arc);
	}

	// 分离轴定理
	bool LaserBox::hitTestSAT(const Player& player, double epsilon) const
	{
		double xMin = std::min(std::min(x1, x2), std::min(x3, x4));
		double xMax = std::max(std::max(x1, x2), std::max(x3, x4));
		if (!HitTest(xMin + (xMax - xMin) / 2.0, xMax - xMin, player.x, player.w, epsilon))
			return false;

		double yMin = std::min(std::min(y1, y2), std::min(y3, y4));
		double yMax = std::max(std::max(y1, y2), std::max(y3, y4));
		if (!HitTest(yMin + (yMax - yMin) / 2.0, yMax - yMin, player.y, player.h, epsilon))
			return false;

		return true;
	}

	// 反向旋转画布，即把激光转回来，再反向旋转自机坐标
	PlayerBox::PlayerBox(const Player& player, const Laser& laser)
	{
		x1 = player.x - player.w / 2.0; y1 = player.y - player.h / 2.0;
		x2 = player.x + player.w / 2.0; y2 = player.y - player.h / 2.0;
		x3 = player.x - player.w / 2.0; y3 = player.y + player.h / 2.0;
		x4 = player.x + player.w / 2.0; y4 = player.y + player.h / 2.0;
		// emmm?
		double arc = laser.arc - 3.14159265358979323846 * 5.0 / 2.0;
		PointRotate(x1, y1, laser.x, laser.y, -arc);
		PointRotate(x2, y2, laser.x, laser.y, -arc);
		PointRotate(x3, y3, laser.x, laser.y, -arc);
		PointRotate(x4, y4, laser.x, laser.y, -arc);
	}

	// 分离轴定理
	bool PlayerBox::hitTestSAT(const Laser& laser, double epsilon) const
	{
		double xMin = std::min(std::min(x1, x2), std::min(x3, x4));
		double xMax = std::max(std::max(x1, x2), std::max(x3, x4));
		if (!HitTest(xMin + (xMax - xMin) / 2.0, xMax - xMin, laser.x, laser.w, epsilon))
			return false;

		double yMin = std::min(std::min(y1, y2), std::min(y3, y4));
		double yMax = std::max(std::max(y1, y2), std::max(y3, y4));
		if (!HitTest(yMin + (yMax - yMin) / 2.0, yMax - yMin, laser.y + laser.h / 2.0, laser.h, epsilon))
			return false;

		return true;
	}
}

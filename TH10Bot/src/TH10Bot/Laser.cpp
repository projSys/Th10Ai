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



	// ƽ���У�һ����(x, y)�������(cx, cy)˳ʱ����תa�Ⱥ������
	// xx = (x - cx) * cos(-a) - (y - cy) * sin(-a) + cx;
	// yy = (x - cx) * sin(-a) + (y - cy) * cos(-a) + cy;
	// ƽ���У�һ����(x, y)�������(cx, cy)��ʱ����תa�Ⱥ������
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
		// emmm...��˵���˭������
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = Rotate(laser.getTopLeft(), C, radianC);
		topRight = Rotate(laser.getTopRight(), C, radianC);
		bottomLeft = Rotate(laser.getBottomLeft(), C, radianC);
		bottomRight = Rotate(laser.getBottomRight(), C, radianC);
	}

	// �����ᶨ��
	bool LaserBox::collide(const Player& player) const
	{
		// ͶӰ��X��
		float_t minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
		float_t maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, player.x, player.width))
			return false;

		// ͶӰ��Y��
		float_t minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
		float_t maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, player.y, player.height))
			return false;

		return true;
	}

	// ������ת���������Ѽ���ת�������ٷ�����ת�Ի�����
	PlayerBox::PlayerBox(const Player& player, const Laser& laser)
	{
		Pointf C = laser.getPos();
		// emmm...��˵���˭������
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = Rotate(player.getTopLeft(), C, -radianC);
		topRight = Rotate(player.getTopRight(), C, -radianC);
		bottomLeft = Rotate(player.getBottomLeft(), C, -radianC);
		bottomRight = Rotate(player.getBottomRight(), C, -radianC);
	}

	// �����ᶨ��
	bool PlayerBox::collide(const Laser& laser) const
	{
		// ͶӰ��X��
		float_t minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
		float_t maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, laser.x, laser.width))
			return false;

		// ͶӰ��Y��
		float_t minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
		float_t maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, laser.y + laser.height / 2.0f, laser.height))
			return false;

		return true;
	}
}

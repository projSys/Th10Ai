#include "libTH10AI/Common.h"
#include "libTH10AI/Entity/Laser.h"

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

	// ���Ӧ��û������ɣ�
	std::pair<bool, float_t> Laser::willCollideWith(const Entity& other) const
	{
		std::pair<Pointf, float_t> footPoint = calcFootPoint(other.getPosition());
		Laser temp = *this;
		temp.setPosition(footPoint.first);
		if (temp.collide(other))
			return std::make_pair(true, footPoint.second);
		else
			return std::make_pair(false, 0.0f);
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
		// emmm...��˵���˭������
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = Rotate(laser.getTopLeft(), C, radianC);
		topRight = Rotate(laser.getTopRight(), C, radianC);
		bottomLeft = Rotate(laser.getBottomLeft(), C, radianC);
		bottomRight = Rotate(laser.getBottomRight(), C, radianC);
	}

	// �����ᶨ��
	bool LaserBox::collide(const Entity& other) const
	{
		// ͶӰ��X��
		float_t minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
		float_t maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, other.x, other.width))
			return false;

		// ͶӰ��Y��
		float_t minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
		float_t maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, other.y, other.height))
			return false;

		return true;
	}

	// ������ת���������Ѽ���ת�������ٷ�����ת�Ի�����
	EntityBox::EntityBox(const Entity& entity, const Laser& laser)
	{
		Pointf C = laser.getPosition();
		// emmm...��˵���˭������
		float_t radianC = laser.arc - static_cast<float_t>(M_PI) * 5.0f / 2.0f;
		topLeft = Rotate(entity.getTopLeft(), C, -radianC);
		topRight = Rotate(entity.getTopRight(), C, -radianC);
		bottomLeft = Rotate(entity.getBottomLeft(), C, -radianC);
		bottomRight = Rotate(entity.getBottomRight(), C, -radianC);
	}

	// �����ᶨ��
	bool EntityBox::collide(const Laser& other) const
	{
		// ͶӰ��X��
		float_t minX = std::min(std::min(topLeft.x, topRight.x), std::min(bottomLeft.x, bottomRight.x));
		float_t maxX = std::max(std::max(topLeft.x, topRight.x), std::max(bottomLeft.x, bottomRight.x));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, other.x, other.width))
			return false;

		// ͶӰ��Y��
		float_t minY = std::min(std::min(topLeft.y, topRight.y), std::min(bottomLeft.y, bottomRight.y));
		float_t maxY = std::max(std::max(topLeft.y, topRight.y), std::max(bottomLeft.y, bottomRight.y));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, other.y + other.height / 2.0f, other.height))
			return false;

		return true;
	}
}

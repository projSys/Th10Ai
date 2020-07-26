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

	// ���Ӧ��û������ɣ�
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
		leftTop = Rotate(laser.getLeftTop(), C, radianC);
		rightTop = Rotate(laser.getRightTop(), C, radianC);
		leftBottom = Rotate(laser.getLeftBottom(), C, radianC);
		rightBottom = Rotate(laser.getRightBottom(), C, radianC);
	}

	// �����ᶨ��
	bool LaserBox::collide(const Entity& other) const
	{
		// ͶӰ��X��
		float_t minX = std::min(std::min(leftTop.x, rightTop.x), std::min(leftBottom.x, rightBottom.x));
		float_t maxX = std::max(std::max(leftTop.x, rightTop.x), std::max(leftBottom.x, rightBottom.x));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, other.x, other.width))
			return false;

		// ͶӰ��Y��
		float_t minY = std::min(std::min(leftTop.y, rightTop.y), std::min(leftBottom.y, rightBottom.y));
		float_t maxY = std::max(std::max(leftTop.y, rightTop.y), std::max(leftBottom.y, rightBottom.y));
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
		leftTop = Rotate(entity.getLeftTop(), C, -radianC);
		rightTop = Rotate(entity.getRightTop(), C, -radianC);
		leftBottom = Rotate(entity.getLeftBottom(), C, -radianC);
		rightBottom = Rotate(entity.getRightBottom(), C, -radianC);
	}

	// �����ᶨ��
	bool EntityBox::collide(const Laser& other) const
	{
		// ͶӰ��X��
		float_t minX = std::min(std::min(leftTop.x, rightTop.x), std::min(leftBottom.x, rightBottom.x));
		float_t maxX = std::max(std::max(leftTop.x, rightTop.x), std::max(leftBottom.x, rightBottom.x));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minX + (maxX - minX) / 2.0f, maxX - minX, other.x, other.width))
			return false;

		// ͶӰ��Y��
		float_t minY = std::min(std::min(leftTop.y, rightTop.y), std::min(leftBottom.y, rightBottom.y));
		float_t maxY = std::max(std::max(leftTop.y, rightTop.y), std::max(leftBottom.y, rightBottom.y));
		// ���2���߶��Ƿ��ص�
		if (!Collide(minY + (maxY - minY) / 2.0f, maxY - minY, other.y + other.height / 2.0f, other.height))
			return false;

		return true;
	}
}

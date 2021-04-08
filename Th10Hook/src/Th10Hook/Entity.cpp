#include "Th10Hook/Entity.h"

#include <math.h>
#include <cmath>

#include "Th10Hook/Math/AABB.h"

namespace th
{
	Entity::Entity()
	{
	}

	Entity::Entity(const vec2& pos0, const vec2& delta0, const vec2& size0) :
		pos(pos0), delta(delta0), size(size0)
	{
	}

	void Entity::updateExtra()
	{
		if (!delta.isZero())
			deltaV = delta.verticalize().normalize();
	}

	float_t Entity::distance(const Entity& other) const
	{
		return (pos - other.pos).length();
	}

	bool Entity::collide(const Entity& other) const
	{
		return std::abs(pos.x - other.pos.x) < (size.x + other.size.x) / 2
			&& std::abs(pos.y - other.pos.y) < (size.y + other.size.y) / 2;
		//return AABB(*this).collide(AABB(other));
	}

	bool Entity::collide(const Entity& other, float_t epsilon) const
	{
		return std::abs(pos.x - other.pos.x) < (size.x + other.size.x) / 2 + epsilon
			&& std::abs(pos.y - other.pos.y) < (size.y + other.size.y) / 2 + epsilon;
	}

	struct FootPoint
	{
		float_t k;
		vec2 pos;
	};

	// ���ȣ���һϵ��k����ֱ�ߵ������յ�ֱ�ΪA��x1�� y1����B��x2�� y2����ֱ����һ��ΪC��x0�� y0��������ΪD������ k = |AD| / |AB|��
	// �� k * AB = AD = AC + CD���� AB * CD = 0������ k * AB * AB = AC * AB���� k = AC * AB / ��AB * AB����
	// �������꣬���� k = ((x0 - x1) * (x2 - x1) + (y0 - y1) * (y2 - y1)) / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	// �� x = x1 + k * (x2 - x1); y = y1 + k * (y2 - y1);
	FootPoint CalcFootPoint(const vec2& A, const vec2& B, const vec2& C)
	{
		FootPoint footPoint = {};

		float_t dxBA = B.x - A.x;
		float_t dyBA = B.y - A.y;
		if (TypeTraits<float_t>::Equals(dxBA, 0)
			&& TypeTraits<float_t>::Equals(dyBA, 0))
		{
			footPoint.k = 0;
			footPoint.pos = A;
			return footPoint;
		}

		float_t dxCA = C.x - A.x;
		float_t dyCA = C.y - A.y;
		footPoint.k = (dxCA * dxBA + dyCA * dyBA) / (dxBA * dxBA + dyBA * dyBA);
		footPoint.pos.x = A.x + footPoint.k * dxBA;
		footPoint.pos.y = A.y + footPoint.k * dyBA;
		return footPoint;
	}

	bool Entity::collide2(const Entity& other) const
	{
		// ���㵽��ǰ֡��ǰһ֮֡���߶ε������
		Entity prev = *this;
		prev.advance(-1);
		FootPoint footPoint = CalcFootPoint(prev.pos, pos, other.pos);
		if (footPoint.k > 1)	// ��δ���ﵱǰ֡
			footPoint.k = 1;
		if (footPoint.k < 0)	// �Ѿ�Զ��һ֡����
			footPoint.k = 0;
		prev.advance(footPoint.k);
		return prev.collide(other);
	}

	bool Entity::collide3(const Entity& other, float_t frame) const
	{
		// ���㵽��ǰ֡��ǰһ֮֡���߶ε������
		Entity temp = *this;
		temp.advance(frame - 1);
		vec2 prevPos = temp.pos;	// ��ǰһ֡������Ϊ��׼
		temp.advance(1);
		vec2 curPos = temp.pos;
		FootPoint footPoint = CalcFootPoint(prevPos, curPos, other.pos);
		if (footPoint.k > 1)	// ��δ���ﵱǰ֡
			footPoint.k = 1;
		if (footPoint.k < 0)	// �Ѿ�Զ��һ֡����
			footPoint.k = 0;
		temp.pos = prevPos;
		temp.advance(footPoint.k);
		return temp.collide(other);
	}

	bool Overlap(const AABB& left, const AABB& right, const vec2& axis)
	{
		// a��b = |a||b|cos��
		// |b| = 1
		// a��b = |a|cos��
		// |a|cos�ȼ�a��b�ϵ�ͶӰ
		float_t proj1 = left.leftTop.dot(axis);
		float_t proj2 = left.rightTop.dot(axis);
		float_t proj3 = left.leftBottom.dot(axis);
		float_t proj4 = left.rightBottom.dot(axis);

		float_t proj5 = right.leftTop.dot(axis);
		float_t proj6 = right.rightTop.dot(axis);
		float_t proj7 = right.leftBottom.dot(axis);
		float_t proj8 = right.rightBottom.dot(axis);

		float_t min1 = std::min({ proj1, proj2, proj3, proj4 });
		float_t max1 = std::max({ proj1, proj2, proj3, proj4 });

		float_t min2 = std::min({ proj5, proj6, proj7, proj8 });
		float_t max2 = std::max({ proj5, proj6, proj7, proj8 });

		//return !(max1 < min2 || max2 < min1);
		return max1 > min2 && max2 > min1;
	}

	bool Entity::willCollideWith(const Entity& other) const
	{
		if (delta.isZero())
			return collide(other);

		return Overlap(AABB(*this), AABB(other), deltaV);
	}

	bool Entity::isHolding() const
	{
		return delta.isZero();
	}

	vec2 Entity::getNextPos() const
	{
		return pos + delta;
	}

	void Entity::advance(float_t frame)
	{
		pos += (delta * frame);
	}

	vec2 Entity::getLeftTop() const
	{
		return vec2(pos.x - size.x / 2, pos.y - size.y / 2);
	}

	vec2 Entity::getRightTop() const
	{
		return vec2(pos.x + size.x / 2, pos.y - size.y / 2);
	}

	vec2 Entity::getLeftBottom() const
	{
		return vec2(pos.x - size.x / 2, pos.y + size.y / 2);
	}

	vec2 Entity::getRightBottom() const
	{
		return vec2(pos.x + size.x / 2, pos.y + size.y / 2);
	}
}

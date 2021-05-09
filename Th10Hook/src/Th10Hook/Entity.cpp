#include "Th10Hook/Entity.h"

#include <math.h>
#include <cmath>

#include "Th10Hook/Math/CollisionDetection.h"

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
		aabb.update(*this);
		if (!delta.isZero())
			deltaV = delta.verticalize().normalize();
	}

	bool Entity::collide(const Entity& other) const
	{
		return std::abs(pos.x - other.pos.x) < (size.x + other.size.x) / _F(2.0)
			&& std::abs(pos.y - other.pos.y) < (size.y + other.size.y) / _F(2.0);
		//return AABB(*this).collide(AABB(other));
	}

	bool Entity::willCollideWith(const Entity& other) const
	{
		// ������Ϊ���ƶ����������ص���
		// Ȼ��ֻ��Ҫ�ټ�����ƶ������Ĵ�ֱ�����Ƿ��ص�
		if (!delta.isZero())
			//return CollisionDetection::CollideSAT(AABB(*this), AABB(other),
			//	delta.verticalize().normalize());
			return CollisionDetection::CollideSAT(aabb, other.aabb, deltaV);
		else
			return collide(other);
	}

	float_t Entity::distance(const Entity& other) const
	{
		return (pos - other.pos).length();
	}

	bool Entity::isHolding() const
	{
		return delta.isZero();
	}

	void Entity::advance(float_t frame)
	{
		pos += (delta * frame);
	}

	vec2 Entity::getLeftTop() const
	{
		return vec2(pos.x - size.x / _F(2.0), pos.y - size.y / _F(2.0));
	}

	vec2 Entity::getRightTop() const
	{
		return vec2(pos.x + size.x / _F(2.0), pos.y - size.y / _F(2.0));
	}

	vec2 Entity::getLeftBottom() const
	{
		return vec2(pos.x - size.x / _F(2.0), pos.y + size.y / _F(2.0));
	}

	vec2 Entity::getRightBottom() const
	{
		return vec2(pos.x + size.x / _F(2.0), pos.y + size.y / _F(2.0));
	}
}

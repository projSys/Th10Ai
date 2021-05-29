#include "Th10Ai/Entity.h"

#include <math.h>
#include <cmath>

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
		{
			deltaV = delta.verticalize().normalize();
			projV = Projection(aabb, deltaV);
		}
	}

	bool Entity::collide(const Entity& other) const
	{
		return std::abs(pos.x - other.pos.x) < (size.x + other.size.x) / _F(2.0)
			&& std::abs(pos.y - other.pos.y) < (size.y + other.size.y) / _F(2.0);
		//return AABB(*this).collide(AABB(other));
	}

	bool Entity::willCollideWith(const Entity& other) const
	{
		// 可以认为与移动向量总是重叠的
		// 然后只需要检测在移动向量的垂直向量上是否重叠
		if (!delta.isZero())
		{
			//vec2 deltaV = delta.verticalize().normalize();
			//return Projection(AABB(*this), deltaV).overlap(
			//	Projection(AABB(other), deltaV));
			return projV.overlap(Projection(other.aabb, deltaV));
		}
		else
		{
			return aabb.collide(other.aabb);
		}
	}

	float_t Entity::distance(const Entity& other) const
	{
		return (pos - other.pos).length();
	}

	bool Entity::isHolding() const
	{
		return delta.isZero();
	}

	void Entity::advance(int_t frame)
	{
		pos += (delta * static_cast<float_t>(frame));
	}

	vec2 Entity::getTopLeft() const
	{
		return vec2(pos.x - size.x / _F(2.0), pos.y - size.y / _F(2.0));
	}

	vec2 Entity::getTopRight() const
	{
		return vec2(pos.x + size.x / _F(2.0), pos.y - size.y / _F(2.0));
	}

	vec2 Entity::getBottomLeft() const
	{
		return vec2(pos.x - size.x / _F(2.0), pos.y + size.y / _F(2.0));
	}

	vec2 Entity::getBottomRight() const
	{
		return vec2(pos.x + size.x / _F(2.0), pos.y + size.y / _F(2.0));
	}
}

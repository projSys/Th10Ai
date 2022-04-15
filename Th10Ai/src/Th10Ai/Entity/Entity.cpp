#include "Th10Ai/Entity/Entity.h"

namespace th
{
	Entity::Entity()
	{
	}

	void Entity::updateExtra()
	{
		updateAABB();

		if (!delta.isZero())
		{
			deltaV = delta.verticalize().normalize();
			projV = project(deltaV);
		}
	}

	//bool Entity::collide(const Entity& other) const
	//{
	//	return std::abs(pos.x - other.pos.x) < (size.x + other.size.x) / _F(2.0)
	//		&& std::abs(pos.y - other.pos.y) < (size.y + other.size.y) / _F(2.0);
	//}

	// 严重错误：并不都是直线运动
	bool Entity::willCollideWith(const AABB& other) const
	{
		// 可以认为与移动向量总是重叠的
		// 然后只需要检测在移动向量的垂直向量上是否重叠
		if (!delta.isZero())
		{
			//vec2 deltaV = delta.verticalize().normalize();
			//return project(deltaV).overlap(other.project(deltaV));
			return projV.overlap(other.project(deltaV));
		}
		else
		{
			return collide(other);
		}
	}

	float_t Entity::distance(const Entity& other) const
	{
		return (pos - other.pos).length();
	}

	void Entity::setPosition(const vec2& pos0)
	{
		pos = pos0;
		updateAABB();
	}

	void Entity::advance(int_t frame)
	{
		vec2 offset = delta * static_cast<float_t>(frame);
		pos += offset;
		translate(offset);
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

	void Entity::updateAABB()
	{
		topLeft = getTopLeft();
		topRight = getTopRight();
		bottomLeft = getBottomLeft();
		bottomRight = getBottomRight();
	}
}

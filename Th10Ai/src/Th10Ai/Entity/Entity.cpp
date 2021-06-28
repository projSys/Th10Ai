#include "Th10Ai/Entity/Entity.h"

namespace th
{
	Entity::Entity()
	{
	}

	void Entity::updateExtra()
	{
		updateAABB();

		if (!m_delta.isZero())
		{
			m_deltaV = m_delta.verticalize().normalize();
			m_projV = project(m_deltaV);
		}
	}

	//bool Entity::collide(const Entity& other) const
	//{
	//	return std::abs(m_pos.x - other.m_pos.x) < (m_size.x + other.m_size.x) / _F(2.0)
	//		&& std::abs(m_pos.y - other.m_pos.y) < (m_size.y + other.m_size.y) / _F(2.0);
	//}

	// 严重错误：并不都是直线运动
	bool Entity::willCollideWith(const AABB& other) const
	{
		// 可以认为与移动向量总是重叠的
		// 然后只需要检测在移动向量的垂直向量上是否重叠
		if (!m_delta.isZero())
		{
			//vec2 deltaV = m_delta.verticalize().normalize();
			//return project(deltaV).overlap(other.project(deltaV));
			return m_projV.overlap(other.project(m_deltaV));
		}
		else
		{
			return collide(other);
		}
	}

	float_t Entity::distance(const Entity& other) const
	{
		return (m_pos - other.m_pos).length();
	}

	void Entity::setPosition(const vec2& pos0)
	{
		m_pos = pos0;
		updateAABB();
	}

	void Entity::advance(int_t frame)
	{
		vec2 offset = m_delta * static_cast<float_t>(frame);
		m_pos += offset;
		translate(offset);
	}

	vec2 Entity::getTopLeft() const
	{
		return vec2(m_pos.x - m_size.x / _F(2.0), m_pos.y - m_size.y / _F(2.0));
	}

	vec2 Entity::getTopRight() const
	{
		return vec2(m_pos.x + m_size.x / _F(2.0), m_pos.y - m_size.y / _F(2.0));
	}

	vec2 Entity::getBottomLeft() const
	{
		return vec2(m_pos.x - m_size.x / _F(2.0), m_pos.y + m_size.y / _F(2.0));
	}

	vec2 Entity::getBottomRight() const
	{
		return vec2(m_pos.x + m_size.x / _F(2.0), m_pos.y + m_size.y / _F(2.0));
	}

	void Entity::updateAABB()
	{
		m_topLeft = getTopLeft();
		m_topRight = getTopRight();
		m_bottomLeft = getBottomLeft();
		m_bottomRight = getBottomRight();
	}
}

#include "Th10Ai/Entity/Entity.h"

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

	bool Entity::willCollideWith(const AABB& other) const
	{
		// ������Ϊ���ƶ����������ص���
		// Ȼ��ֻ��Ҫ������ƶ������Ĵ�ֱ�������Ƿ��ص�
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
		m_topLeft = getTopLeft();
		m_topRight = getTopRight();
		m_bottomLeft = getBottomLeft();
		m_bottomRight = getBottomRight();
	}
}

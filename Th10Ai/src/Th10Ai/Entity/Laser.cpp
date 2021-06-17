#include "Th10Ai/Entity/Laser.h"

#include <math.h>
#include <cmath>

#include "Th10Ai/Entity/Entity.h"

namespace th
{
	Laser::Laser() :
		m_arc()
	{
	}

	void Laser::updateExtra()
	{
		updateOBB();

		if (!m_delta.isZero())
		{
			m_deltaV = m_delta.verticalize().normalize();
			m_projV = project(m_deltaV);
		}
	}

	bool Laser::willCollideWith(const AABB& other) const
	{
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

	float_t Laser::distance(const Entity& entity) const
	{
		return (m_pos - entity.m_pos).length();
	}

	void Laser::setPosition(const vec2& pos0)
	{
		m_pos = pos0;
		updateOBB();
	}

	void Laser::advance(int_t frame)
	{
		vec2 offset = m_delta * static_cast<float_t>(frame);
		m_pos += offset;
		translate(offset);
	}

	vec2 Laser::getTopLeft() const
	{
		return vec2(m_pos.x - m_size.x / _F(2.0), m_pos.y);
	}

	vec2 Laser::getTopRight() const
	{
		return vec2(m_pos.x + m_size.x / _F(2.0), m_pos.y);
	}

	vec2 Laser::getBottomLeft() const
	{
		return vec2(m_pos.x - m_size.x / _F(2.0), m_pos.y + m_size.y);
	}

	vec2 Laser::getBottomRight() const
	{
		return vec2(m_pos.x + m_size.x / _F(2.0), m_pos.y + m_size.y);
	}

	float_t Laser::getRadian() const
	{
		// emmm...��˵���˭������
		float_t radian = m_arc - static_cast<float_t>(M_PI) * _F(5.0) / _F(2.0);
		return radian;
	}

	void Laser::updateOBB()
	{
		float_t radian = getRadian();
		float_t sinVal = std::sin(radian);
		float_t cosVal = std::cos(radian);
		m_topLeft = m_pos + (getTopLeft() - m_pos).rotate(sinVal, cosVal);
		m_topRight = m_pos + (getTopRight() - m_pos).rotate(sinVal, cosVal);
		m_bottomLeft = m_pos + (getBottomLeft() - m_pos).rotate(sinVal, cosVal);
		m_bottomRight = m_pos + (getBottomRight() - m_pos).rotate(sinVal, cosVal);
		m_projAxisX = vec2(_F(1.0), _F(0.0)).rotate(sinVal, cosVal);
		m_projAxisY = vec2(_F(0.0), _F(1.0)).rotate(sinVal, cosVal);
	}
}

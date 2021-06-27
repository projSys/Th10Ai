#include "Th10Ai/Entity/Bullet.h"

namespace th
{
	Bullet::Bullet() :
		m_status(),
		m_type(),
		m_id()
	{
	}

	Bullet::Bullet(const BulletRaw* raw, uint_t id)
	{
		m_pos.x = raw->x;
		m_pos.y = raw->y;
		m_delta.x = raw->dx;
		m_delta.y = raw->dy;
		m_size.x = raw->width;
		m_size.y = raw->height;
		m_status = raw->status;
		//m_type = raw->type;
		m_type = static_cast<int_t>(raw->width);
		m_id = id;
	}
}

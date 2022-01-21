#include "Th10Ai/Entity/Item.h"

namespace th
{
	Item::Item() :
		m_status(),
		m_type(),
		m_id()
	{
	}

	Item::Item(const ItemRaw* raw, uint_t id)
	{
		m_pos.x = raw->x;
		m_pos.y = raw->y;
		m_delta.x = raw->dx;
		m_delta.y = raw->dy;
		// 点没有宽度和高度，自机靠近点时会自动收取，为了方便显示设定为6
		m_size.x = _F(6.0);
		m_size.y = _F(6.0);
		m_status = raw->status;
		m_type = raw->type;
		m_id = id;
	}
}

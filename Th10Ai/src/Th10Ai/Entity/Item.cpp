#include "Th10Ai/Entity/Item.h"

namespace th
{
	Item::Item() :
		m_status(),
		m_type(),
		m_id(0)
	{
	}

	Item::Item(const ItemRaw* raw, uint_t id)
	{
		pos.x = raw->x;
		pos.y = raw->y;
		delta.x = raw->dx;
		delta.y = raw->dy;
		// ��û�п�Ⱥ͸߶ȣ��Ի�������ʱ���Զ���ȡ��Ϊ�˷�����ʾ�趨Ϊ6
		size.x = _F(6.0);
		size.y = _F(6.0);
		m_status = raw->status;
		m_type = raw->type;
		m_id = id;
	}
}

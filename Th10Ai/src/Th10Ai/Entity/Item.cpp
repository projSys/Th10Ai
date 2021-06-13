#include "Th10Ai/Entity/Item.h"

namespace th
{
	Item::Item() :
		status(),
		type()
	{
	}

	Item::Item(const ItemRaw* raw)
	{
		pos.x = raw->x;
		pos.y = raw->y;
		delta.x = raw->dx;
		delta.y = raw->dy;
		// ��û�п�Ⱥ͸߶ȣ��Ի�������ʱ���Զ���ȡ��Ϊ�˷�����ʾ�趨Ϊ6
		size.x = _F(6.0);
		size.y = _F(6.0);
		status = raw->status;
		type = raw->type;
	}
}

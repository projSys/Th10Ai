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
		// 点没有宽度和高度，自机靠近点时会自动收取，为了方便显示设定为6
		size.x = _F(6.0);
		size.y = _F(6.0);
		status = raw->status;
		type = raw->type;
	}
}

#include "Th10Ai/Item.h"

namespace th
{
	Item::Item() :
		type()
	{
	}

	Item::Item(const ItemData& data) :
		Entity(data),
		type(data.type)
	{
	}
}

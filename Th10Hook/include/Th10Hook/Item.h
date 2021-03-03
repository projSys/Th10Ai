#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Entity.h"

namespace th
{
	class Item :
		public Entity
	{
	public:
		Item();
		explicit Item(const ItemData& data);

		int_t type;
	};
}

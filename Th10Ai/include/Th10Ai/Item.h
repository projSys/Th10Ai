#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Th10Data.h"
#include "Th10Ai/Entity.h"

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

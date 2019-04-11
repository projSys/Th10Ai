#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	struct Item :
		Entity
	{
		Item advance(float_t frame) const;

		int_t type;
	};
}

#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	struct Enemy :
		Entity
	{
		Enemy advance(float_t frame) const;
	};
}

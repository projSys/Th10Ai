#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	struct Bullet :
		Entity
	{
		Bullet advance(float_t frame) const;
	};
}

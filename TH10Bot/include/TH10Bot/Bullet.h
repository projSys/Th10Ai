#pragma once

#include "TH10Bot/Entity.h"

namespace th
{
	class Bullet :
		public Entity
	{
	public:
		Bullet advance(const Pointf& pos) const;
		Bullet advance(float_t frame) const;
	};
}

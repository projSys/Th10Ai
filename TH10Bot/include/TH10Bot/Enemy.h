#pragma once

#include "TH10Bot/Entity.h"

namespace th
{
	class Enemy :
		public Entity
	{
	public:
		Enemy advance(const Pointf& pos) const;
		Enemy advance(float_t frame) const;
	};
}

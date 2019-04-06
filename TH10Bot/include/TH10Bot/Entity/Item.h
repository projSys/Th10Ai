#pragma once

#include "TH10Bot/Entity/Entity.h"

namespace th
{
	class Item :
		public Entity
	{
	public:
		//Item() :
		//	type() {}
		//Item(float_t x0, float_t y0, float_t dx0, float_t dy0, float_t width0, float_t height0) :
		//	Entity(x0, y0, dx0, dy0, width0, height0), type() {}

		int32_t type;
	};
}

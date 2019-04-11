#include "TH10Bot/Common.h"
#include "TH10Bot/Entity/Item.h"

namespace th
{
	Item Item::advance(float_t frame) const
	{
		Item adv = *this;
		adv.x += (dx * frame);
		adv.y += (dy * frame);
		return adv;
	}
}

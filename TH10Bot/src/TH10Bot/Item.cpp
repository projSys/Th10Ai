#include "TH10Bot/Common.h"
#include "TH10Bot/Item.h"

namespace th
{
	Item Item::advance(const Pointf& pos) const
	{
		Item ret = *this;
		ret.x = pos.x;
		ret.y = pos.y;
		return ret;
	}

	Item Item::advance(float_t frame) const
	{
		Item ret = *this;
		ret.x += (dx * frame);
		ret.y += (dy * frame);
		return ret;
	}
}

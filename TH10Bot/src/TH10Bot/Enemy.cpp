#include "TH10Bot/Common.h"
#include "TH10Bot/Enemy.h"

namespace th
{
	Enemy Enemy::advance(const Pointf& pos) const
	{
		Enemy ret = *this;
		ret.x = pos.x;
		ret.y = pos.y;
		return ret;
	}

	Enemy Enemy::advance(float_t frame) const
	{
		Enemy ret = *this;
		ret.x += (dx * frame);
		ret.y += (dy * frame);
		return ret;
	}
}

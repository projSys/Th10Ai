#include "TH10Bot/Common.h"
#include "TH10Bot/Entity/Enemy.h"

namespace th
{
	Enemy Enemy::advance(float_t frame) const
	{
		Enemy adv = *this;
		adv.x += (dx * frame);
		adv.y += (dy * frame);
		return adv;
	}
}

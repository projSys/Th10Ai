#include "TH10Bot/Common.h"
#include "TH10Bot/Entity/Player.h"

namespace th
{
	Player Player::advance(float_t frame) const
	{
		Player adv = *this;
		adv.x += (dx * frame);
		adv.y += (dy * frame);
		return adv;
	}
}

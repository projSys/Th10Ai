#include "TH10Bot/Common.h"
#include "TH10Bot/Entity/Bullet.h"

namespace th
{
	Bullet Bullet::advance(float_t frame) const
	{
		Bullet adv = *this;
		adv.x += (dx * frame);
		adv.y += (dy * frame);
		return adv;
	}
}

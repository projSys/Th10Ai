#include "TH10Bot/Common.h"
#include "TH10Bot/Bullet.h"

namespace th
{
	Bullet Bullet::advance(const Pointf& pos) const
	{
		Bullet ret = *this;
		ret.x = pos.x;
		ret.y = pos.y;
		return ret;
	}

	Bullet Bullet::advance(float_t frame) const
	{
		Bullet ret = *this;
		ret.x += (dx * frame);
		ret.y += (dy * frame);
		return ret;
	}
}

#include "Th10Hook/Bullet.h"

namespace th
{
	Bullet::Bullet() :
		id(0)
	{
	}

	Bullet::Bullet(const BulletData& data) :
		Entity(data),
		id(data.id)
	{
	}
}

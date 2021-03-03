#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Entity.h"

namespace th
{
	class Bullet :
		public Entity
	{
	public:
		Bullet();
		explicit Bullet(const BulletData& data);

		int_t id;
	};
}

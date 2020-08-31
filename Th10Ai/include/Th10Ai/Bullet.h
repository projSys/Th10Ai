#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity.h"

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

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

		int_t id;
	};
}

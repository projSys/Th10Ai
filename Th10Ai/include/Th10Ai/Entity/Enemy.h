#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/Entity.h"
#include "Th10Ai/Th10Raw.h"

namespace th
{
	class Enemy :
		public Entity
	{
	public:
		Enemy();
		explicit Enemy(const EnemyRaw* raw);

		bool isBoss() const;
	};
}

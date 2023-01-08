#pragma once

#include "Th10Base/Common.h"

#include "Th10Base/Entity.h"
#include "Th10Base/Th10Raws.h"

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

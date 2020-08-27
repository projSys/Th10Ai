#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Th10Data.h"
#include "Th10Ai/Entity.h"

namespace th
{
	class Enemy :
		public Entity
	{
	public:
		Enemy();
		explicit Enemy(const EnemyData& data);

		bool isBoss() const;
	};
}

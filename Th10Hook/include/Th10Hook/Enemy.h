#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Entity.h"

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

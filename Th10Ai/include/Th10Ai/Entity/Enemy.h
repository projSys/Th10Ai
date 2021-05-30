#pragma once

#include "Th10Ai/Common.h"

#include "Th10Ai/Entity/Entity.h"

namespace th
{
	class Enemy :
		public Entity
	{
	public:
		Enemy();

		bool isBoss() const;
	};
}

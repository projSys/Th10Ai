#pragma once

#include "Th10Hook/Entity.h"

namespace th
{
	class Enemy :
		public Entity
	{
	public:
		bool isBoss() const;
	};
}

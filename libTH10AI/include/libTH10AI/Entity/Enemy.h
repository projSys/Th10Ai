#pragma once

#include "libTH10AI/Entity/Entity.h"

namespace th
{
	class Enemy :
		public Entity
	{
	public:
		bool isBoss() const;
	};
}

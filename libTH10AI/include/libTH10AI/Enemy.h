#pragma once

#include "libTH10AI/Entity.h"

namespace th
{
	class Enemy :
		public Entity
	{
	public:
		bool isBoss() const;
	};
}

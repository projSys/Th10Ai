#pragma once

#include "Th10Ai/Common.h"

#include <vector>

#include "Th10Ai/Entity/Player.h"
#include "Th10Ai/Entity/Item.h"
#include "Th10Ai/Entity/Enemy.h"
#include "Th10Ai/Entity/Bullet.h"
#include "Th10Ai/Entity/Laser.h"

namespace th
{
	class Th10Apis
	{
	public:
		static bool GetGlobalVarTo(GlobalVar& globalVar);
		static bool GetPlayerTo(Player& player);
		static bool GetItemsTo(std::vector<Item>& items);
		static bool GetEnemiesTo(std::vector<Enemy>& enemies);
		static bool GetBulletsTo(std::vector<Bullet>& bullets);
		static bool GetLasersTo(std::vector<Laser>& lasers);
	};
}

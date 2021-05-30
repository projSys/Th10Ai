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
		static bool GetPlayer(Player& player);
		static bool GetItems(std::vector<Item>& items);
		static bool GetEnemies(std::vector<Enemy>& enemies);
		static bool GetBullets(std::vector<Bullet>& bullets);
		static bool GetLasers(std::vector<Laser>& lasers);
	};
}

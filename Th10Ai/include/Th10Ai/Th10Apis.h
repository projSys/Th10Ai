#pragma once

#include "Th10Ai/Common.h"

#include <vector>

#include "Th10Ai/Player.h"
#include "Th10Ai/Item.h"
#include "Th10Ai/Enemy.h"
#include "Th10Ai/Bullet.h"
#include "Th10Ai/Laser.h"

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

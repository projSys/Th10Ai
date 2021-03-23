#pragma once

#include "Th10Hook/Common.h"

#include <vector>

#include "Th10Hook/Player.h"
#include "Th10Hook/Item.h"
#include "Th10Hook/Enemy.h"
#include "Th10Hook/Bullet.h"
#include "Th10Hook/Laser.h"

namespace th
{
	class Th10Hook
	{
	public:
		static bool GetPlayer(Player& player);
		static bool GetItems(std::vector<Item>& items);
		static bool GetEnemies(std::vector<Enemy>& enemies);
		static bool GetBullets(std::vector<Bullet>& bullets);
		static bool GetLasers(std::vector<Laser>& lasers);
	};
}

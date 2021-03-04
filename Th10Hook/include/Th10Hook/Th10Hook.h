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
		static bool ReadPlayer(Player& player);
		static bool ReadItems(std::vector<Item>& items);
		static bool ReadEnemies(std::vector<Enemy>& enemies);
		static bool ReadBullets(std::vector<Bullet>& bullets);
		static bool ReadLasers(std::vector<Laser>& lasers);
	};
}

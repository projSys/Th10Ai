#pragma once

#include <vector>

#include "TH10Bot/Entity/Player.h"
#include "TH10Bot/Entity/Item.h"
#include "TH10Bot/Entity/Enemy.h"
#include "TH10Bot/Entity/Bullet.h"
#include "TH10Bot/Entity/Laser.h"

namespace th
{
	class Th10Data
	{
	public:
		Th10Data();

		Player player;
		std::vector<Item> items;
		std::vector<Enemy> enemies;
		std::vector<Bullet> bullets;
		std::vector<Laser> lasers;
	};
}

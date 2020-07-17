#pragma once

#include "Th10Hook/Status.h"

namespace th
{
	// https://github.com/binvec/TH10_DataReversing
	class Reader2
	{
	public:
		static int_t ReadPlayer(Player_t& player);
		static int_t ReadItems(Item_t items[]);
		static int_t ReadEnemies(Enemy_t enemies[]);
		static int_t ReadBullets(Bullet_t bullets[]);
		static int_t ReadLasers(Laser_t lasers[]);

	private:
		template <typename T>
		static T ReadMemory(uint_t address)
		{
			return *reinterpret_cast<T*>(address);
		}
	};
}

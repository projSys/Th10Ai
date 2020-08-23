#pragma once

#include "Th10Hook/Th10Data.h"

namespace th
{
	// https://github.com/binvec/TH10_DataReversing
	class Reader
	{
	public:
		static int_t ReadPlayer(PlayerData& player);
		static int_t ReadItems(ItemData items[]);
		static int_t ReadEnemies(EnemyData enemies[]);
		static int_t ReadBullets(BulletData bullets[]);
		static int_t ReadLasers(LaserData lasers[]);

	private:
		template <typename T>
		static T ReadMemory(uint_t address)
		{
			return *reinterpret_cast<T*>(address);
		}
	};
}

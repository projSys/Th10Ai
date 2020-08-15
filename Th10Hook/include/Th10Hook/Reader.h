#pragma once

#include "Th10Hook/RawStatus.h"

namespace th
{
	// https://github.com/binvec/TH10_DataReversing
	class Reader
	{
	public:
		static int_t ReadPlayer(RawPlayer& player);
		static int_t ReadItems(RawItem items[]);
		static int_t ReadEnemies(RawEnemy enemies[]);
		static int_t ReadBullets(RawBullet bullets[]);
		static int_t ReadLasers(RawLaser lasers[]);

	private:
		template <typename T>
		static T ReadMemory(uint_t address)
		{
			return *reinterpret_cast<T*>(address);
		}
	};
}

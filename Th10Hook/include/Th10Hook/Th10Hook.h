#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Th10Data.h"

namespace th
{
	class Th10Listener
	{
	public:
		virtual ~Th10Listener() = default;
	};

	// https://github.com/binvec/TH10_DataReversing
	class Th10Hook
	{
	public:
		Th10Hook(Th10Listener* listener);

		int_t readPlayer(PlayerData& player);
		int_t readItems(ItemData items[]);
		int_t readEnemies(EnemyData enemies[]);
		int_t readBullets(BulletData bullets[]);
		int_t readLasers(LaserData lasers[]);

	private:
		template <typename T>
		static T ReadMemory(uint_t address)
		{
			return *reinterpret_cast<T*>(address);
		}

		Th10Listener* m_listener;
	};
}

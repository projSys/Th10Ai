#pragma once

#include "Th10Hook/Common.h"

#include "Th10Hook/Th10Types.h"

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

		uint_t readPlayer(PlayerData& player);
		uint_t readItems(ItemData items[]);
		uint_t readEnemies(EnemyData enemies[]);
		uint_t readBullets(BulletData bullets[]);
		uint_t readLasers(LaserData lasers[]);

	private:
		template <typename T>
		static T ReadMemory(uint_t address)
		{
			return *reinterpret_cast<T*>(address);
		}

		Th10Listener* m_listener;
	};
}

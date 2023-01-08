#pragma once

#include "Th10Hook/Common.h"

#include <Th10Base/Th10Raws.h>
#include <Th10Base/SharedMemory.h>
#include <Th10Base/Player.h>
#include <Th10Base/Item.h>
#include <Th10Base/Enemy.h>
#include <Th10Base/Bullet.h>
#include <Th10Base/Laser.h>

namespace th
{
	class Th10Apis
	{
	public:
		static bool GetGlobalVarTo(GlobalVar& globalVar);
		static bool GetPlayerTo(Player& player);
		static bool GetItemsTo(ManagedVector<Item>& items);
		static bool GetEnemiesTo(ManagedVector<Enemy>& enemies);
		static bool GetBulletsTo(ManagedVector<Bullet>& bullets);
		static bool GetLasersTo(ManagedVector<Laser>& lasers);
	};
}

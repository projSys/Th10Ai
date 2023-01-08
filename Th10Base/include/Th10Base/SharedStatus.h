#pragma once

#include "Th10Base/Common.h"

#include "Th10Base/SharedMemory.h"
#include "Th10Base/Player.h"
#include "Th10Base/Item.h"
#include "Th10Base/Enemy.h"
#include "Th10Base/Bullet.h"
#include "Th10Base/Laser.h"

namespace th
{
	class SharedStatus
	{
	public:
		explicit SharedStatus(SharedMemory& sharedMemory);

	private:
		VoidAllocator m_voidAlloc;

	public:
		GlobalVar globalVar;
		Player player;
		ManagedVector<Item> items;
		ManagedVector<Enemy> enemies;
		ManagedVector<Bullet> bullets;
		ManagedVector<Laser> lasers;
	};
}

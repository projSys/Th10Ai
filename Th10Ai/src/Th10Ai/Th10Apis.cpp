#include "Th10Ai/Th10Apis.h"

#include <iomanip>

#include "Th10Ai/Th10Raw.h"

namespace th
{
	template <typename T>
	T ReadMemory(uint_t address)
	{
		return *reinterpret_cast<T*>(address);
	}

	bool Th10Apis::GetGlobalVarTo(GlobalVar& globalVar)
	{
		globalVar.power = ReadMemory<int32_t>(0x00474C48) / _F(20.0);
		globalVar.character = ReadMemory<int32_t>(0x00474C68);
		globalVar.player = ReadMemory<int32_t>(0x00474C70) + 1;
		globalVar.itemGetRange = ReadMemory<float32_t>(0x00476FB0) + globalVar.character * 4;
		//if (player.isSlow())
		//	globalVar.itemGetRange *= _F(2.5);

		globalVar.stageFrame = ReadMemory<int32_t>(0x00474C88);

		return true;
	}

	bool Th10Apis::GetPlayerTo(Player& player)
	{
		//uint32_t baseAddr = ReadMemory<uint32_t>(0x00477834);
		//if (baseAddr == 0)
		//	return false;

		//player.x = ReadMemory<float32_t>(baseAddr + 0x3C0);
		//player.y = ReadMemory<float32_t>(baseAddr + 0x3C4);
		//player.dx = ReadMemory<int32_t>(baseAddr + 0x3F0) / _F(100.0);
		//player.dy = ReadMemory<int32_t>(baseAddr + 0x3F4) / _F(100.0);
		//player.width = ReadMemory<float32_t>(baseAddr + 0x41C) * _F(2.0);
		//player.height = player.width;
		//player.status = ReadMemory<int32_t>(baseAddr + 0x458);
		//player.invincibleTime = ReadMemory<int32_t>(baseAddr + 0x4310);
		//player.slow = ReadMemory<int32_t>(baseAddr + 0x4474);

		player.powers = ReadMemory<int32_t>(0x00474C48) / _F(20.0);
		player.character = ReadMemory<int32_t>(0x00474C68);
		player.lives = ReadMemory<int32_t>(0x00474C70) + 1;
		player.itemGetRange = ReadMemory<float32_t>(0x00476FB0) + player.character * 4;
		if (player.isSlow())
			player.itemGetRange *= _F(2.5);

		player.stageFrame = ReadMemory<int32_t>(0x00474C88);

		const PlayerRaw* raw = ReadMemory<const PlayerRaw*>(0x00477834);
		if (raw == nullptr)
			return false;

		player.set(raw);

		return true;
	}

	bool Th10Apis::GetItemsTo(std::vector<Item>& items)
	{
		//uint32_t baseAddr = ReadMemory<uint32_t>(0x00477818);
		//if (baseAddr == 0)
		//	return false;

		//uint32_t ebp = baseAddr + 0x3C0;

		//for (int_t i = 0; i < 2000; ++i)
		//{
		//	uint32_t eax = ReadMemory<uint32_t>(ebp + 0x30);
		//	// 点的类型分为以下几种
		//	// eax == 0 不存在
		//	// eax == 1 正常的可收点
		//	// eax == 2 放B消除子弹产生的点
		//	// eax == 3 到达收点线、放B等自动回收的点
		//	// eax == 4 到达点的收取范围，自动回收的点
		//	if (eax == 1)
		//	{
		//		Item item;
		//		item.pos.x = ReadMemory<float32_t>(ebp);
		//		item.pos.y = ReadMemory<float32_t>(ebp + 0x4);
		//		item.delta.x = ReadMemory<float32_t>(ebp + 0xC);
		//		item.delta.y = ReadMemory<float32_t>(ebp + 0x10);
		//		// 点没有宽度和高度，自机靠近点时会自动收取，为了方便显示设定为6
		//		item.size.x = _F(6.0);
		//		item.size.y = _F(6.0);
		//		item.type = ReadMemory<int32_t>(ebp + 0x34);

		//		//item.id = i;

		//		items.push_back(item);
		//	}
		//	ebp += 0x3F0;
		//}

		const ItemContainer* container = ReadMemory<const ItemContainer*>(0x00477818);
		if (container == nullptr)
			return false;

		for (uint_t i = 0; i < ITEM_MAX_COUNT; ++i)
		{
			const ItemRaw* raw = &(container->items[i]);
			if (raw->status == 1)
				items.emplace_back(raw, i);
		}

		return true;
	}

	bool Th10Apis::GetEnemiesTo(std::vector<Enemy>& enemies)
	{
		//uint32_t baseAddr = ReadMemory<uint32_t>(0x00477704);
		//if (baseAddr == 0)
		//	return false;

		//uint32_t objBase = ReadMemory<uint32_t>(baseAddr + 0x58);
		//if (objBase == 0)
		//	return false;

		//while (true)
		//{
		//	uint32_t objAddr = ReadMemory<uint32_t>(objBase) + 0x103C;
		//	uint32_t objNext = ReadMemory<uint32_t>(objBase + 0x4);

		//	uint32_t t = ReadMemory<uint32_t>(objAddr + 0x1444);
		//	if ((t & 0x40) == 0 && (t & 0x12) == 0)
		//	{
		//		Enemy enemy;
		//		enemy.pos.x = ReadMemory<float32_t>(objAddr + 0x2C);
		//		enemy.pos.y = ReadMemory<float32_t>(objAddr + 0x30);
		//		enemy.delta.x = ReadMemory<float32_t>(objAddr + 0x38);
		//		enemy.delta.y = ReadMemory<float32_t>(objAddr + 0x3C);
		//		enemy.size.x = ReadMemory<float32_t>(objAddr + 0xB8);
		//		enemy.size.y = ReadMemory<float32_t>(objAddr + 0xBC);

		//		//enemy.id = static_cast<int_t>(objAddr);
		//		//enemy.type = static_cast<int_t>(std::round(enemy.width));

		//		enemies.push_back(enemy);
		//	}
		//	if (objNext == 0)
		//		break;
		//	objBase = objNext;
		//}

		const EnemyContainer* container = ReadMemory<const EnemyContainer*>(0x00477704);
		if (container == nullptr)
			return false;

		const EnemyElement* element = container->head;
		if (element == nullptr)
			return false;

		do
		{
			EnemyRaw* raw = element->raw;
			if ((raw->status & 0x40) == 0 && (raw->status & 0x12) == 0)
				enemies.emplace_back(raw);

			element = element->next;
		} while (element != nullptr);

		return true;
	}

	bool Th10Apis::GetBulletsTo(std::vector<Bullet>& bullets)
	{
		//uint32_t baseAddr = ReadMemory<uint32_t>(0x004776F0);
		//if (baseAddr == 0)
		//	return false;

		//uint32_t ebx = baseAddr + 0x60;

		//for (int_t i = 0; i < BULLET_MAX_COUNT - 1; ++i)
		//{
		//	//uint32_t bp = ReadMemory<uint32_t>(ebx + 0x446);
		//	//if ((bp & 0x0000FFFF) != 0)
		//	int16_t bp = ReadMemory<int16_t>(ebx + 0x446);
		//	if (bp != 0)
		//	{
		//		// emmm...你说这个谁懂啊？
		//		//uint32_t eax = ReadMemory<uint32_t>(0x00477810);
		//		//if (eax != 0)
		//		//{
		//		//	eax = ReadMemory<uint32_t>(eax + 0x58);
		//		//	if ((eax & 0x00000400) == 0)
		//		//	{
		//				Bullet bullet;
		//				bullet.pos.x = ReadMemory<float32_t>(ebx + 0x3B4);
		//				bullet.pos.y = ReadMemory<float32_t>(ebx + 0x3B8);
		//				bullet.delta.x = ReadMemory<float32_t>(ebx + 0x3C0);
		//				bullet.delta.y = ReadMemory<float32_t>(ebx + 0x3C4);
		//				bullet.size.x = ReadMemory<float32_t>(ebx + 0x3F0);
		//				bullet.size.y = ReadMemory<float32_t>(ebx + 0x3F4);

		//				bullet.id = i;
		//				//bullet.type = static_cast<int_t>(std::round(bullet.width));
		//				// 真心不如用宽度来当类型
		//				int32_t type = ReadMemory<int32_t>(ebx + 0x460);
		//				//int32_t type = ReadMemory<int32_t>(ebx + 0x7EC);

		//				bullets.push_back(bullet);
		//		//	}
		//		//}
		//	}
		//	ebx += 0x7F0;
		//}

		const BulletContainer* container = ReadMemory<const BulletContainer*>(0x004776F0);
		if (container == nullptr)
			return false;

		// 最后一个是空子弹，status == 5，其他全0
		for (uint_t i = 0; i < BULLET_MAX_COUNT - 1; ++i)
		{
			const BulletRaw* raw = &(container->bullets[i]);
			if (raw->status != 0)
				bullets.emplace_back(raw, i);
		}
		if (bullets.size() != container->bulletCount)
			std::cout << "读取到的子弹数量不一致。" << std::endl;

		return true;
	}

	bool Th10Apis::GetLasersTo(std::vector<Laser>& lasers)
	{
		//uint32_t baseAddr = ReadMemory<uint32_t>(0x0047781C);
		//if (baseAddr == 0)
		//	return false;

		//uint32_t objAddr = ReadMemory<uint32_t>(baseAddr + 0x18);
		//if (objAddr == 0)
		//	return false;

		//while (true)
		//{
		//	uint32_t objNext = ReadMemory<uint32_t>(objAddr + 0x8);

		//	Laser laser;
		//	laser.pos.x = ReadMemory<float32_t>(objAddr + 0x24);
		//	laser.pos.y = ReadMemory<float32_t>(objAddr + 0x28);
		//	laser.delta.x = ReadMemory<float32_t>(objAddr + 0x30);
		//	laser.delta.y = ReadMemory<float32_t>(objAddr + 0x34);
		//	laser.arc = ReadMemory<float32_t>(objAddr + 0x3C);
		//	// 高度在前，宽度在后
		//	laser.size.y = ReadMemory<float32_t>(objAddr + 0x40);
		//	laser.size.x = ReadMemory<float32_t>(objAddr + 0x44);

		//	//laser.id = static_cast<int_t>(objAddr);
		//	//laser.type = static_cast<int_t>(std::round(laser.width));

		//	lasers.push_back(laser);

		//	if (objNext == 0)
		//		break;
		//	objAddr = objNext;
		//}

		const LaserContainer* container = ReadMemory<const LaserContainer*>(0x0047781C);
		if (container == nullptr)
			return false;

		const LaserRaw* raw = container->head;
		if (raw == nullptr)
			return false;

		do
		{
			lasers.emplace_back(raw);

			raw = raw->next;
		} while (raw != nullptr);

		return true;
	}
}

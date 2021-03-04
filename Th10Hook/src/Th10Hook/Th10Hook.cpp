#include "Th10Hook/Th10Hook.h"

#include "Th10Hook/Th10Types.h"

namespace th
{
	template <typename T>
	T ReadMemory(uint_t address)
	{
		return *reinterpret_cast<T*>(address);
	}

	bool Th10Hook::ReadPlayer(Player& player)
	{
		player = {};

		RawPlayer* raw = ReadMemory<RawPlayer*>(0x00477834);
		if (raw == nullptr)
			return false;

		player.x = raw->x;
		player.y = raw->y;
		player.dx = raw->dx / 100.0;
		player.dy = raw->dy / 100.0;
		player.width = raw->width * 2.0;
		player.height = raw->height * 2.0;
		player.status = raw->status;
		player.invincibleTime = raw->invincibleTime;
		player.slow = raw->slow;

		//std::cout
		//	<< ' ' << player.x
		//	<< ' ' << player.y
		//	<< ' ' << player.dx
		//	<< ' ' << player.dy
		//	<< ' ' << player.width
		//	<< ' ' << player.height
		//	<< ' ' << player.status
		//	<< ' ' << player.invincibleTime
		//	<< ' ' << player.slow
		//	<< std::endl;

		//uint32_t baseAddr = ReadMemory<uint32_t>(0x00477834);
		//if (baseAddr == 0)
		//	return false;

		//player.x = ReadMemory<float32_t>(baseAddr + 0x3C0);
		//player.y = ReadMemory<float32_t>(baseAddr + 0x3C4);
		//player.dx = ReadMemory<int32_t>(baseAddr + 0x3F0) / 100.0;
		//player.dy = ReadMemory<int32_t>(baseAddr + 0x3F4) / 100.0;
		//player.width = ReadMemory<float32_t>(baseAddr + 0x41C) * 2.0;
		//player.height = player.width;
		//player.status = ReadMemory<int32_t>(baseAddr + 0x458);
		//player.invincibleTime = ReadMemory<int32_t>(baseAddr + 0x4310);
		//player.slow = ReadMemory<int32_t>(baseAddr + 0x4474);

		player.powers = ReadMemory<int32_t>(0x00474C48) / 20.0f;
		player.type = ReadMemory<int32_t>(0x00474C68);
		player.lives = ReadMemory<int32_t>(0x00474C70) + 1;
		player.itemObtainRange = ReadMemory<float32_t>(0x00476FB0) + player.type * 4;
		if (player.slow)
			player.itemObtainRange *= 2.5f;

		return true;
	}

	bool Th10Hook::ReadItems(std::vector<Item>& items)
	{
		items.clear();

		uint32_t baseAddr = ReadMemory<uint32_t>(0x00477818);
		if (baseAddr == 0)
			return false;

		uint32_t ebp = baseAddr + 0x3C0;

		for (int_t i = 0; i < 2000; ++i)
		{
			uint32_t eax = ReadMemory<uint32_t>(ebp + 0x30);
			// 点的类型分为以下几种
			// eax == 0 不存在
			// eax == 1 正常的可收点
			// eax == 2 放B消除子弹产生的点
			// eax == 3 到达收点线、放B等自动回收的点
			// eax == 4 到达点的收取范围，自动回收的点
			if (eax == 1)
			{
				Item item;
				item.x = ReadMemory<float32_t>(ebp);
				item.y = ReadMemory<float32_t>(ebp + 0x4);
				item.dx = ReadMemory<float32_t>(ebp + 0xC);
				item.dy = ReadMemory<float32_t>(ebp + 0x10);
				// 点没有宽度和高度，自机靠近点时会自动收取，为了方便显示设定为6
				item.width = 6.0f;
				item.height = item.width;
				item.type = ReadMemory<int32_t>(ebp + 0x34);

				//item.id = i;

				items.push_back(item);
			}
			ebp += 0x3F0;
		}

		return true;
	}

	bool Th10Hook::ReadEnemies(std::vector<Enemy>& enemies)
	{
		enemies.clear();

		uint32_t baseAddr = ReadMemory<uint32_t>(0x00477704);
		if (baseAddr == 0)
			return false;

		uint32_t objBase = ReadMemory<uint32_t>(baseAddr + 0x58);
		if (objBase == 0)
			return false;

		while (true)
		{
			uint32_t objAddr = ReadMemory<uint32_t>(objBase) + 0x103C;
			uint32_t objNext = ReadMemory<uint32_t>(objBase + 0x4);
			uint32_t t = ReadMemory<uint32_t>(objAddr + 0x1444);
			if ((t & 0x40) == 0 && (t & 0x12) == 0)
			{
				Enemy enemy;
				enemy.x = ReadMemory<float32_t>(objAddr + 0x2C);
				enemy.y = ReadMemory<float32_t>(objAddr + 0x30);
				enemy.dx = ReadMemory<float32_t>(objAddr + 0x38);
				enemy.dy = ReadMemory<float32_t>(objAddr + 0x3C);
				enemy.width = ReadMemory<float32_t>(objAddr + 0xB8);
				enemy.height = ReadMemory<float32_t>(objAddr + 0xBC);

				//enemy.id = static_cast<int_t>(objAddr);
				//enemy.type = static_cast<int_t>(std::round(enemy.width));

				enemies.push_back(enemy);
			}
			if (objNext == 0)
				break;
			objBase = objNext;
		}

		return true;
	}

	bool Th10Hook::ReadBullets(std::vector<Bullet>& bullets)
	{
		bullets.clear();

		uint32_t baseAddr = ReadMemory<uint32_t>(0x004776F0);
		if (baseAddr == 0)
			return false;

		uint32_t ebx = baseAddr + 0x60;

		for (int_t i = 0; i < 2000; ++i)
		{
			uint32_t bp = ReadMemory<uint32_t>(ebx + 0x446);
			if ((bp & 0x0000FFFF) != 0)
			{
				uint32_t eax = ReadMemory<uint32_t>(0x00477810);
				if (eax != 0)
				{
					eax = ReadMemory<uint32_t>(eax + 0x58);
					if ((eax & 0x00000400) == 0)
					{
						Bullet bullet;
						bullet.x = ReadMemory<float32_t>(ebx + 0x3B4);
						bullet.y = ReadMemory<float32_t>(ebx + 0x3B8);
						bullet.dx = ReadMemory<float32_t>(ebx + 0x3C0);
						bullet.dy = ReadMemory<float32_t>(ebx + 0x3C4);
						bullet.width = ReadMemory<float32_t>(ebx + 0x3F0);
						bullet.height = ReadMemory<float32_t>(ebx + 0x3F4);

						bullet.id = static_cast<int_t>(ebx);
						//bullet.type = static_cast<int_t>(std::round(bullet.width));

						bullets.push_back(bullet);
					}
				}
			}
			ebx += 0x7F0;
		}

		return true;
	}

	bool Th10Hook::ReadLasers(std::vector<Laser>& lasers)
	{
		lasers.clear();

		uint32_t baseAddr = ReadMemory<uint32_t>(0x0047781C);
		if (baseAddr == 0)
			return false;

		uint32_t objAddr = ReadMemory<uint32_t>(baseAddr + 0x18);
		if (objAddr == 0)
			return false;

		while (true)
		{
			uint32_t objNext = ReadMemory<uint32_t>(objAddr + 0x8);

			Laser laser;
			laser.x = ReadMemory<float32_t>(objAddr + 0x24);
			laser.y = ReadMemory<float32_t>(objAddr + 0x28);
			laser.dx = ReadMemory<float32_t>(objAddr + 0x30);
			laser.dy = ReadMemory<float32_t>(objAddr + 0x34);
			laser.arc = ReadMemory<float32_t>(objAddr + 0x3C);
			laser.height = ReadMemory<float32_t>(objAddr + 0x40);
			laser.width = ReadMemory<float32_t>(objAddr + 0x44);

			//laser.id = static_cast<int_t>(objAddr);
			//laser.type = static_cast<int_t>(std::round(laser.width));

			lasers.push_back(laser);

			if (objNext == 0)
				break;
			objAddr = objNext;
		}

		return true;
	}
}

#include "Th10Hook/Common.h"
#include "Th10Hook/Reader2.h"

namespace th
{
	int_t Reader2::ReadPlayer(Player_t& player)
	{
		player = {};

		uint32_t baseAddr = ReadMemory<uint32_t>(0x00477834);
		if (baseAddr == 0)
			return 0;

		player.x = ReadMemory<float32_t>(baseAddr + 0x3C0);
		player.y = ReadMemory<float32_t>(baseAddr + 0x3C4);
		player.dx = ReadMemory<int32_t>(baseAddr + 0x3F0) / 100.0f;
		player.dy = ReadMemory<int32_t>(baseAddr + 0x3F4) / 100.0f;
		player.width = ReadMemory<float32_t>(baseAddr + 0x41C) * 2.0f;
		player.height = player.width;
		player.status = ReadMemory<int32_t>(baseAddr + 0x458);
		player.invincibleTime = ReadMemory<int32_t>(baseAddr + 0x4310);
		player.slow = ReadMemory<int32_t>(baseAddr + 0x4474);
		player.powers = ReadMemory<int32_t>(0x00474C48) / 20.0f;
		player.type = ReadMemory<int32_t>(0x00474C68);
		player.lives = ReadMemory<int32_t>(0x00474C70) + 1;
		player.itemObtainRange = ReadMemory<float32_t>(0x00476FB0) + player.type * 4;
		if (player.slow)
			player.itemObtainRange *= 2.5f;

		return 1;
	}

	int_t Reader2::ReadItems(Item_t items[])
	{
		int_t count = 0;

		uint32_t baseAddr = ReadMemory<uint32_t>(0x00477818);
		if (baseAddr == 0)
			return 0;

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
				items[count].x = ReadMemory<float32_t>(ebp);
				items[count].y = ReadMemory<float32_t>(ebp + 0x4);
				items[count].dx = ReadMemory<float32_t>(ebp + 0xC);
				items[count].dy = ReadMemory<float32_t>(ebp + 0x10);
				// 点没有宽度和高度，自机靠近点时会自动收取，为了方便显示设定为6
				items[count].width = 6.0f;
				items[count].height = items[count].width;
				items[count].type = ReadMemory<int32_t>(ebp + 0x34);

				//items[count].id = i;

				++count;
			}
			ebp += 0x3F0;
		}

		return count;
	}

	int_t Reader2::ReadEnemies(Enemy_t enemies[])
	{
		int_t count = 0;

		uint32_t baseAddr = ReadMemory<uint32_t>(0x00477704);
		if (baseAddr == 0)
			return 0;

		uint32_t objBase = ReadMemory<uint32_t>(baseAddr + 0x58);
		if (objBase == 0)
			return 0;

		while (true)
		{
			uint32_t objAddr = ReadMemory<uint32_t>(objBase) + 0x103C;
			uint32_t objNext = ReadMemory<uint32_t>(objBase + 0x4);
			uint32_t t = ReadMemory<uint32_t>(objAddr + 0x1444);
			if ((t & 0x40) == 0 && (t & 0x12) == 0)
			{
				enemies[count].x = ReadMemory<float32_t>(objAddr + 0x2C);
				enemies[count].y = ReadMemory<float32_t>(objAddr + 0x30);
				enemies[count].dx = ReadMemory<float32_t>(objAddr + 0x38);
				enemies[count].dy = ReadMemory<float32_t>(objAddr + 0x3C);
				enemies[count].width = ReadMemory<float32_t>(objAddr + 0xB8);
				enemies[count].height = ReadMemory<float32_t>(objAddr + 0xBC);

				//enemies[count].id = static_cast<int_t>(objAddr);
				//enemies[count].type = static_cast<int_t>(std::round(enemies[count].width));

				++count;
			}
			if (objNext == 0)
				break;
			objBase = objNext;
		}

		return count;
	}

	int_t Reader2::ReadBullets(Bullet_t bullets[])
	{
		int_t count = 0;

		uint32_t baseAddr = ReadMemory<uint32_t>(0x004776F0);
		if (baseAddr == 0)
			return 0;

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
						bullets[count].x = ReadMemory<float32_t>(ebx + 0x3B4);
						bullets[count].y = ReadMemory<float32_t>(ebx + 0x3B8);
						bullets[count].dx = ReadMemory<float32_t>(ebx + 0x3C0);
						bullets[count].dy = ReadMemory<float32_t>(ebx + 0x3C4);
						bullets[count].width = ReadMemory<float32_t>(ebx + 0x3F0);
						bullets[count].height = ReadMemory<float32_t>(ebx + 0x3F4);

						//bullets[count].id = i;
						//bullets[count].type = static_cast<int_t>(std::round(bullets[count].width));

						++count;
					}
				}
			}
			ebx += 0x7F0;
		}

		return count;
	}

	int_t Reader2::ReadLasers(Laser_t lasers[])
	{
		int_t count = 0;

		uint32_t baseAddr = ReadMemory<uint32_t>(0x0047781C);
		if (baseAddr == 0)
			return 0;

		uint32_t objAddr = ReadMemory<uint32_t>(baseAddr + 0x18);
		if (objAddr == 0)
			return 0;

		while (true)
		{
			uint32_t objNext = ReadMemory<uint32_t>(objAddr + 0x8);

			lasers[count].x = ReadMemory<float32_t>(objAddr + 0x24);
			lasers[count].y = ReadMemory<float32_t>(objAddr + 0x28);
			lasers[count].dx = ReadMemory<float32_t>(objAddr + 0x30);
			lasers[count].dy = ReadMemory<float32_t>(objAddr + 0x34);
			lasers[count].arc = ReadMemory<float32_t>(objAddr + 0x3C);
			lasers[count].height = ReadMemory<float32_t>(objAddr + 0x40);
			lasers[count].width = ReadMemory<float32_t>(objAddr + 0x44);

			//lasers[count].id = static_cast<int_t>(objAddr);
			//lasers[count].type = static_cast<int_t>(std::round(lasers[count].width));

			++count;

			if (objNext == 0)
				break;
			objAddr = objNext;
		}

		return count;
	}
}

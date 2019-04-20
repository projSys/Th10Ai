#include "TH10Bot/Common.h"
#include "TH10Bot/TH10Reader.h"

namespace th
{
	TH10Reader::TH10Reader(Process& process) :
		m_process(process)
	{
	}

	bool TH10Reader::getPlayer(Player& player) noexcept
	{
		byte_t buffer[0x458 - 0x3C0 + 4];
		player = {};

		uint32_t baseAddr = readMemory<uint32_t>(0x00477834);
		if (baseAddr == 0)
			return false;

		//player.x = readMemory<float32_t>(baseAddr + 0x3C0);
		//player.y = readMemory<float32_t>(baseAddr + 0x3C4);
		//player.dx = readMemory<int32_t>(baseAddr + 0x3F0) / 100.0f;
		//player.dy = readMemory<int32_t>(baseAddr + 0x3F4) / 100.0f;
		//player.width = readMemory<float32_t>(baseAddr + 0x41C) * 2.0f;
		//player.height = player.width;
		//player.slow = readMemory<int32_t>(baseAddr + 0x4474);
		//player.powers = readMemory<int32_t>(0x00474C48) / 20.0f;
		//player.type = readMemory<int32_t>(0x00474C68);
		//player.life = readMemory<int32_t>(0x00474C70) + 1;
		//player.itemObtainRange = readMemory<float32_t>(0x00476FB0) + player.type * 4;
		//if (player.slow)
		//	player.itemObtainRange *= 2.5f;
		//player.status = readMemory<int32_t>(baseAddr + 0x458);
		//player.invinibleTime = readMemory<int32_t>(baseAddr + 0x4310);
		readMemoryBuffer(baseAddr + 0x3C0, buffer, sizeof(buffer));
		player.x = readBuffer<float32_t>(buffer - 0x3C0 + 0x3C0);
		player.y = readBuffer<float32_t>(buffer - 0x3C0 + 0x3C4);
		player.dx = readBuffer<int32_t>(buffer - 0x3C0 + 0x3F0) / 100.0f;
		player.dy = readBuffer<int32_t>(buffer - 0x3C0 + 0x3F4) / 100.0f;
		player.width = readBuffer<float32_t>(buffer - 0x3C0 + 0x41C) * 2.0f;	// 2还是4啊？
		player.height = player.width = 5.0f;	// 设大点不容易撞
		//player.slow = readMemory<int32_t>(baseAddr + 0x4474);
		//player.powers = readMemory<int32_t>(0x00474C48) / 20.0f;
		//player.type = readMemory<int32_t>(0x00474C68);
		//player.life = readMemory<int32_t>(0x00474C70) + 1;
		//player.itemObtainRange = readMemory<float32_t>(0x00476FB0) + player.type * 4;
		//if (player.slow)
		//	player.itemObtainRange *= 2.5f;
		player.status = readBuffer<int32_t>(buffer - 0x3C0 + 0x458);
		//player.invinibleTime = readMemory<int32_t>(baseAddr + 0x4310);

		return true;
	}

	bool TH10Reader::getItems(std::vector<Item>& items) noexcept
	{
		items.clear();

		uint32_t baseAddr = readMemory<uint32_t>(0x00477818);
		if (baseAddr == 0)
			return false;

		readMemoryBuffer(baseAddr + 0x3C0, m_buffer, 0x3F0 * /*2000*/200);
		byte_t* ebp = m_buffer;
		for (int_t i = 0; i < /*2000*/200; ++i)
		{
			uint32_t eax = readBuffer<uint32_t>(ebp + 0x30);
			// 点的类型分为以下几种
			// eax == 0 不存在
			// eax == 1 正常的可收点
			// eax == 2 放B消除子弹产生的点
			// eax == 3 到达收点线、放B等自动回收的点
			// eax == 4 到达点的收取范围，自动回收的点
			if (eax == 1)
			{
				Item item;
				item.x = readBuffer<float32_t>(ebp);
				item.y = readBuffer<float32_t>(ebp + 0x4);
				item.dx = readBuffer<float32_t>(ebp + 0xC);
				item.dy = readBuffer<float32_t>(ebp + 0x10);
				// 点没有宽度和高度，自机靠近点时会自动收取，为了方便显示设定为6
				item.height = item.width = 6.0f;
				item.type = readBuffer<int32_t>(ebp + 0x34);
				items.push_back(item);
			}
			ebp += 0x3F0;
		}

		return true;
	}

	bool TH10Reader::getEnemies(std::vector<Enemy>& enemies) noexcept
	{
		byte_t buffer[0xBC - 0x2C + 4];
		enemies.clear();

		uint32_t baseAddr = readMemory<uint32_t>(0x00477704);
		if (baseAddr == 0)
			return false;

		uint32_t objBase = readMemory<uint32_t>(baseAddr + 0x58);
		if (objBase == 0)
			return false;

		while (true)
		{
			uint32_t objAddr = readMemory<uint32_t>(objBase) + 0x103C;
			uint32_t objNext = readMemory<uint32_t>(objBase + 0x4);
			uint32_t t = readMemory<uint32_t>(objAddr + 0x1444);
			if ((t & 0x40) == 0 && (t & 0x12) == 0)
			{
				Enemy enemy;
				//enemy.x = readMemory<float32_t>(objAddr + 0x2C);
				//enemy.y = readMemory<float32_t>(objAddr + 0x30);
				//enemy.dx = readMemory<float32_t>(objAddr + 0x38);
				//enemy.dy = readMemory<float32_t>(objAddr + 0x3C);
				//enemy.width = readMemory<float32_t>(objAddr + 0xB8);
				//enemy.height = readMemory<float32_t>(objAddr + 0xBC);
				readMemoryBuffer(objAddr + 0x2C, buffer, sizeof(buffer));
				enemy.x = readBuffer<float32_t>(buffer - 0x2C + 0x2C);
				enemy.y = readBuffer<float32_t>(buffer - 0x2C + 0x30);
				enemy.dx = readBuffer<float32_t>(buffer - 0x2C + 0x38);
				enemy.dy = readBuffer<float32_t>(buffer - 0x2C + 0x3C);
				enemy.width = readBuffer<float32_t>(buffer - 0x2C + 0xB8);
				enemy.height = readBuffer<float32_t>(buffer - 0x2C + 0xBC);
				enemies.push_back(enemy);
			}
			if (objNext == 0)
				break;
			objBase = objNext;
		}

		return true;
	}

	bool TH10Reader::getBullets(std::vector<Bullet>& bullets) noexcept
	{
		bullets.clear();

		uint32_t baseAddr = readMemory<uint32_t>(0x004776F0);
		if (baseAddr == 0)
			return false;

		uint32_t eax = readMemory<uint32_t>(0x00477810);
		if (eax == 0)
			return false;
		eax = readMemory<uint32_t>(eax + 0x58);
		if ((eax & 0x00000400) != 0)
			return false;

		readMemoryBuffer(baseAddr + 0x60, m_buffer, 0x7F0 * 2000);
		byte_t* ebx = m_buffer;
		for (int_t i = 0; i < 2000; ++i)
		{
			uint32_t bp = readBuffer<uint32_t>(ebx + 0x446);
			if ((bp & 0x0000FFFF) != 0)
			{
				Bullet bullet;
				bullet.x = readBuffer<float32_t>(ebx + 0x3B4);
				bullet.y = readBuffer<float32_t>(ebx + 0x3B8);
				bullet.dx = readBuffer<float32_t>(ebx + 0x3C0);
				bullet.dy = readBuffer<float32_t>(ebx + 0x3C4);
				bullet.width = readBuffer<float32_t>(ebx + 0x3F0);
				bullet.height = readBuffer<float32_t>(ebx + 0x3F4);
				bullets.push_back(bullet);
			}
			ebx += 0x7F0;
		}

		return true;
	}

	bool TH10Reader::getLasers(std::vector<Laser>& lasers) noexcept
	{
		byte_t buffer[0x44 + 4];
		lasers.clear();

		uint32_t baseAddr = readMemory<uint32_t>(0x0047781C);
		if (baseAddr == 0)
			return false;

		uint32_t objAddr = readMemory<uint32_t>(baseAddr + 0x18);
		if (objAddr == 0)
			return false;

		while (true)
		{
			readMemoryBuffer(objAddr, buffer, sizeof(buffer));
			//uint32_t objNext = readMemory<uint32_t>(objAddr + 0x8);
			uint32_t objNext = readBuffer<uint32_t>(buffer + 0x8);

			Laser laser;
			//laser.x = readMemory<float32_t>(objAddr + 0x24);
			//laser.y = readMemory<float32_t>(objAddr + 0x28);
			//laser.dx = readMemory<float32_t>(objAddr + 0x30);
			//laser.dy = readMemory<float32_t>(objAddr + 0x34);
			//laser.arc = readMemory<float32_t>(objAddr + 0x3C);
			//laser.height = readMemory<float32_t>(objAddr + 0x40);
			//laser.width = readMemory<float32_t>(objAddr + 0x44);
			laser.x = readBuffer<float32_t>(buffer + 0x24);
			laser.y = readBuffer<float32_t>(buffer + 0x28);
			laser.dx = readBuffer<float32_t>(buffer + 0x30);
			laser.dy = readBuffer<float32_t>(buffer + 0x34);
			laser.arc = readBuffer<float32_t>(buffer + 0x3C);
			laser.height = readBuffer<float32_t>(buffer + 0x40);
			laser.width = readBuffer<float32_t>(buffer + 0x44);
			lasers.push_back(laser);

			if (objNext == 0)
				break;
			objAddr = objNext;
		}
		return true;
	}
}

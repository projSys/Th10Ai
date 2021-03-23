#include "Th10Hook/Th10Hook.h"

#include "Th10Hook/Th10Types.h"

namespace th
{
	template <typename T>
	T ReadMemory(uint_t address)
	{
		return *reinterpret_cast<T*>(address);
	}

	bool Th10Hook::GetPlayer(Player& player)
	{
		player = {};

		RawPlayer* raw = ReadMemory<RawPlayer*>(0x00477834);
		if (raw == nullptr)
			return false;

		player.pos.x = raw->x;
		player.pos.y = raw->y;
		player.delta.x = static_cast<float_t>(raw->dx) / 100;
		player.delta.y = static_cast<float_t>(raw->dy) / 100;
		player.size.x = raw->width * 2;
		player.size.y = raw->height * 2;
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
		//player.width = ReadMemory<float32_t>(baseAddr + 0x41C) * 2;
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

		player.frame = ReadMemory<int32_t>(0x00474C88);

		return true;
	}

	bool Th10Hook::GetItems(std::vector<Item>& items)
	{
		items.clear();

		uint32_t baseAddr = ReadMemory<uint32_t>(0x00477818);
		if (baseAddr == 0)
			return false;

		uint32_t ebp = baseAddr + 0x3C0;

		for (int_t i = 0; i < 2000; ++i)
		{
			uint32_t eax = ReadMemory<uint32_t>(ebp + 0x30);
			// ������ͷ�Ϊ���¼���
			// eax == 0 ������
			// eax == 1 �����Ŀ��յ�
			// eax == 2 ��B�����ӵ������ĵ�
			// eax == 3 �����յ��ߡ���B���Զ����յĵ�
			// eax == 4 ��������ȡ��Χ���Զ����յĵ�
			if (eax == 1)
			{
				Item item;
				item.pos.x = ReadMemory<float32_t>(ebp);
				item.pos.y = ReadMemory<float32_t>(ebp + 0x4);
				item.delta.x = ReadMemory<float32_t>(ebp + 0xC);
				item.delta.y = ReadMemory<float32_t>(ebp + 0x10);
				// ��û�п�Ⱥ͸߶ȣ��Ի�������ʱ���Զ���ȡ��Ϊ�˷�����ʾ�趨Ϊ6
				item.size.x = 6;
				item.size.y = 6;
				item.type = ReadMemory<int32_t>(ebp + 0x34);

				//item.id = i;

				items.push_back(item);
			}
			ebp += 0x3F0;
		}

		return true;
	}

	bool Th10Hook::GetEnemies(std::vector<Enemy>& enemies)
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
				enemy.pos.x = ReadMemory<float32_t>(objAddr + 0x2C);
				enemy.pos.y = ReadMemory<float32_t>(objAddr + 0x30);
				enemy.delta.x = ReadMemory<float32_t>(objAddr + 0x38);
				enemy.delta.y = ReadMemory<float32_t>(objAddr + 0x3C);
				enemy.size.x = ReadMemory<float32_t>(objAddr + 0xB8);
				enemy.size.y = ReadMemory<float32_t>(objAddr + 0xBC);

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

	bool Th10Hook::GetBullets(std::vector<Bullet>& bullets)
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
						bullet.pos.x = ReadMemory<float32_t>(ebx + 0x3B4);
						bullet.pos.y = ReadMemory<float32_t>(ebx + 0x3B8);
						bullet.delta.x = ReadMemory<float32_t>(ebx + 0x3C0);
						bullet.delta.y = ReadMemory<float32_t>(ebx + 0x3C4);
						bullet.size.x = ReadMemory<float32_t>(ebx + 0x3F0);
						bullet.size.y = ReadMemory<float32_t>(ebx + 0x3F4);

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

	bool Th10Hook::GetLasers(std::vector<Laser>& lasers)
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
			laser.pos.x = ReadMemory<float32_t>(objAddr + 0x24);
			laser.pos.y = ReadMemory<float32_t>(objAddr + 0x28);
			laser.delta.x = ReadMemory<float32_t>(objAddr + 0x30);
			laser.delta.y = ReadMemory<float32_t>(objAddr + 0x34);
			laser.arc = ReadMemory<float32_t>(objAddr + 0x3C);
			laser.size.x = ReadMemory<float32_t>(objAddr + 0x40);
			laser.size.y = ReadMemory<float32_t>(objAddr + 0x44);

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

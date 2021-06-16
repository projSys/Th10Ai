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

	bool Th10Apis::GetPlayer(Player& player)
	{
		const PlayerRaw* raw = ReadMemory<const PlayerRaw*>(0x00477834);
		if (raw == nullptr)
			return false;

		player.set(raw);

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

		player.powers = ReadMemory<int32_t>(0x00474C48) / _F(20.0);
		player.character = ReadMemory<int32_t>(0x00474C68);
		player.lives = ReadMemory<int32_t>(0x00474C70) + 1;
		player.itemGetRange = ReadMemory<float32_t>(0x00476FB0) + player.character * 4;
		if (player.isSlow())
			player.itemGetRange *= _F(2.5);

		player.stageFrame = ReadMemory<int32_t>(0x00474C88);

		return true;
	}

	bool Th10Apis::GetItems(std::vector<Item>& items)
	{
		//uint32_t baseAddr = ReadMemory<uint32_t>(0x00477818);
		//if (baseAddr == 0)
		//	return false;

		//uint32_t ebp = baseAddr + 0x3C0;

		//for (int_t i = 0; i < 2000; ++i)
		//{
		//	uint32_t eax = ReadMemory<uint32_t>(ebp + 0x30);
		//	// ������ͷ�Ϊ���¼���
		//	// eax == 0 ������
		//	// eax == 1 �����Ŀ��յ�
		//	// eax == 2 ��B�����ӵ������ĵ�
		//	// eax == 3 �����յ��ߡ���B���Զ����յĵ�
		//	// eax == 4 ��������ȡ��Χ���Զ����յĵ�
		//	if (eax == 1)
		//	{
		//		Item item;
		//		item.m_pos.x = ReadMemory<float32_t>(ebp);
		//		item.m_pos.y = ReadMemory<float32_t>(ebp + 0x4);
		//		item.m_delta.x = ReadMemory<float32_t>(ebp + 0xC);
		//		item.m_delta.y = ReadMemory<float32_t>(ebp + 0x10);
		//		// ��û�п�Ⱥ͸߶ȣ��Ի�������ʱ���Զ���ȡ��Ϊ�˷�����ʾ�趨Ϊ6
		//		item.m_size.x = _F(6.0);
		//		item.m_size.y = _F(6.0);
		//		item.m_type = ReadMemory<int32_t>(ebp + 0x34);

		//		//item.m_id = i;

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

	bool Th10Apis::GetEnemies(std::vector<Enemy>& enemies)
	{
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
				enemy.m_pos.x = ReadMemory<float32_t>(objAddr + 0x2C);
				enemy.m_pos.y = ReadMemory<float32_t>(objAddr + 0x30);
				enemy.m_delta.x = ReadMemory<float32_t>(objAddr + 0x38);
				enemy.m_delta.y = ReadMemory<float32_t>(objAddr + 0x3C);
				enemy.m_size.x = ReadMemory<float32_t>(objAddr + 0xB8);
				enemy.m_size.y = ReadMemory<float32_t>(objAddr + 0xBC);

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

	bool Th10Apis::GetBullets(std::vector<Bullet>& bullets)
	{
		uint32_t baseAddr = ReadMemory<uint32_t>(0x004776F0);
		if (baseAddr == 0)
			return false;

		uint32_t ebx = baseAddr + 0x60;

		for (int_t i = 0; i < BULLET_MAX_COUNT - 1; ++i)
		{
			//uint32_t bp = ReadMemory<uint32_t>(ebx + 0x446);
			//if ((bp & 0x0000FFFF) != 0)
			int16_t bp = ReadMemory<int16_t>(ebx + 0x446);
			if (bp != 0)
			{
				//std::cout << bp << ' ';

				// emmm...��˵���˭������
				//uint32_t eax = ReadMemory<uint32_t>(0x00477810);
				//if (eax != 0)
				//{
				//	eax = ReadMemory<uint32_t>(eax + 0x58);
				//	if ((eax & 0x00000400) == 0)
				//	{
						//std::cout << std::hex << std::uppercase << eax << '\n';

						Bullet bullet;
						bullet.m_pos.x = ReadMemory<float32_t>(ebx + 0x3B4);
						bullet.m_pos.y = ReadMemory<float32_t>(ebx + 0x3B8);
						bullet.m_delta.x = ReadMemory<float32_t>(ebx + 0x3C0);
						bullet.m_delta.y = ReadMemory<float32_t>(ebx + 0x3C4);
						bullet.m_size.x = ReadMemory<float32_t>(ebx + 0x3F0);
						bullet.m_size.y = ReadMemory<float32_t>(ebx + 0x3F4);

						bullet.m_id = i;
						//bullet.type = static_cast<int_t>(std::round(bullet.width));
						// ���Ĳ����ÿ����������
						int32_t type = ReadMemory<int32_t>(ebx + 0x460);
						//int32_t type = ReadMemory<int32_t>(ebx + 0x7EC);
						//std::cout << type << ' ';

						bullets.push_back(bullet);
				//	}
				//}
			}
			ebx += 0x7F0;
		}
		//std::cout << '\n';

		//const BulletContainer* container = ReadMemory<const BulletContainer*>(0x004776F0);
		//if (container == nullptr)
		//	return false;

		// ���һ���ǿ��ӵ���status == 5������ȫ0
		//for (uint_t i = 0; i < BULLET_MAX_COUNT - 1; ++i)
		//{
		//	const BulletRaw* raw = &(container->bullets[i]);

		//}

		return true;
	}

	bool Th10Apis::GetLasers(std::vector<Laser>& lasers)
	{
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
			laser.m_pos.x = ReadMemory<float32_t>(objAddr + 0x24);
			laser.m_pos.y = ReadMemory<float32_t>(objAddr + 0x28);
			laser.m_delta.x = ReadMemory<float32_t>(objAddr + 0x30);
			laser.m_delta.y = ReadMemory<float32_t>(objAddr + 0x34);
			laser.m_arc = ReadMemory<float32_t>(objAddr + 0x3C);
			// �߶���ǰ������ں�
			laser.m_size.y = ReadMemory<float32_t>(objAddr + 0x40);
			laser.m_size.x = ReadMemory<float32_t>(objAddr + 0x44);

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

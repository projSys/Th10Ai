#pragma once

#include <vector>

#include "libTH10AI/Entity/Player.h"
#include "libTH10AI/Entity/Item.h"
#include "libTH10AI/Entity/Enemy.h"
#include "libTH10AI/Entity/Bullet.h"
#include "libTH10AI/Entity/Laser.h"

namespace th
{
	// https://github.com/binvec/TH10_DataReversing
	class Api
	{
	public:
		bool readPlayer(Player& player) noexcept;
		bool readItems(std::vector<Item>& items) noexcept;
		bool readEnemies(std::vector<Enemy>& enemies) noexcept;
		bool readBullets(std::vector<Bullet>& bullets) noexcept;
		bool readLasers(std::vector<Laser>& lasers) noexcept;

	private:
		template <typename T>
		inline T readMemory(uint_t address) noexcept
		{
			return *reinterpret_cast<T*>(address);
			//T data = T();
			//ReadProcessMemory(m_process, reinterpret_cast<LPCVOID>(baseAddress), &data, sizeof(data), nullptr);
			//return data;
		}

		//inline void readMemoryBuffer(uint_t baseAddress, byte_t* buffer, uint_t size) noexcept
		//{
		//	ReadProcessMemory(m_process, reinterpret_cast<LPCVOID>(baseAddress), buffer, size, nullptr);
		//}

		//template <typename T>
		//inline T readBuffer(byte_t* address) noexcept
		//{
		//	return *reinterpret_cast<T*>(address);
		//}
	};
}

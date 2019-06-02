#pragma once

#include <vector>
#include <Base/Process.h>

#include "TH10Bot/Player.h"
#include "TH10Bot/Item.h"
#include "TH10Bot/Enemy.h"
#include "TH10Bot/Bullet.h"
#include "TH10Bot/Laser.h"

namespace th
{
	// https://github.com/binvec/TH10_DataReversing
	class TH10Reader
	{
	public:
		TH10Reader(Process& process);

		bool getPlayer(Player& player) noexcept;
		bool getItems(std::vector<Item>& items) noexcept;
		bool getEnemies(std::vector<Enemy>& enemies) noexcept;
		bool getBullets(std::vector<Bullet>& bullets) noexcept;
		bool getLasers(std::vector<Laser>& lasers) noexcept;

	private:
		template <typename T>
		inline T readMemory(uint_t baseAddress) noexcept
		{
			T data = T();
			ReadProcessMemory(m_process, reinterpret_cast<LPCVOID>(baseAddress), &data, sizeof(data), nullptr);
			return data;
		}

		inline void readMemoryBuffer(uint_t baseAddress, byte_t* buffer, uint_t size) noexcept
		{
			ReadProcessMemory(m_process, reinterpret_cast<LPCVOID>(baseAddress), buffer, size, nullptr);
		}

		template <typename T>
		inline T readBuffer(byte_t* address) noexcept
		{
			return *reinterpret_cast<T*>(address);
		}

		Process& m_process;
		byte_t m_buffer[0x7F0 * 2000];
	};
}

// ¸ÐÐ» https://github.com/binvec/TH10_DataReversing
#pragma once

#include <vector>
#include <Windows/Process.h>

#include "TH10Bot/Entity.h"

namespace th
{
	class TH10Reader
	{
	public:
		TH10Reader(Process& process);

		bool getPlayer(Player& player);
		bool getItems(std::vector<Item>& items);
		bool getEnemies(std::vector<Enemy>& enemies);
		bool getBullets(std::vector<Bullet>& bullets);
		bool getLasers(std::vector<Laser>& lasers);

	private:
		template <typename T>
		inline T readMemory(uintptr_t baseAddr)
		{
			T value = T();
			ReadProcessMemory(m_process, reinterpret_cast<LPCVOID>(baseAddr), &value, sizeof(value), nullptr);
			return value;
		}

		Process& m_process;
	};
}

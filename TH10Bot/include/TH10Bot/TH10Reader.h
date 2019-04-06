#pragma once

#include <vector>
#include <Windows/Process.h>

#include "TH10Bot/Entity/Player.h"
#include "TH10Bot/Entity/Item.h"
#include "TH10Bot/Entity/Enemy.h"
#include "TH10Bot/Entity/Bullet.h"
#include "TH10Bot/Entity/Laser.h"

namespace th
{
	// https://github.com/binvec/TH10_DataReversing
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
		T readMemory(uint_t baseAddress)
		{
			T data = T();
			ReadProcessMemory(m_process, reinterpret_cast<LPCVOID>(baseAddress), &data, sizeof(data), nullptr);
			return data;
		}

		Process& m_process;
	};
}

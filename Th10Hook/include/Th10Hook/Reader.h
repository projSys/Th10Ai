#pragma once

//#include <Windows/Process.h>

#include "Th10Hook/Data.h"

namespace th
{
	// https://github.com/binvec/TH10_DataReversing
	class Reader
	{
	public:
		//Reader(Process& process);

		bool readPlayer(Player& player) noexcept;
		bool readItems(vector<Item>& items) noexcept;
		bool readEnemies(vector<Enemy>& enemies) noexcept;
		bool readBullets(vector<Bullet>& bullets) noexcept;
		bool readLasers(vector<Laser>& lasers) noexcept;

	private:
		// 读内存太慢了
		//template <typename T>
		//T readMemory(uint_t address) noexcept
		//{
		//	T data();
		//	ReadProcessMemory(m_process, reinterpret_cast<LPCVOID>(address), &data, sizeof(data), nullptr);
		//	return data;
		//}

		// DLL注入后直接撸地址吧
		template <typename T>
		T readMemory(uint_t address) noexcept
		{
			return *reinterpret_cast<T*>(address);
		}

		//Process& m_process;
	};
}

#pragma once

#include "Th10Ai/Common.h"

#include <thread>
#include <atomic>
#include <boost/interprocess/managed_windows_shared_memory.hpp>

#include "Th10Ai/SharedData.h"
#include "Th10Ai/Status.h"
#include "Th10Ai/Scene.h"

namespace th
{
	namespace interprocess = boost::interprocess;

	class Th10Ai
	{
	public:
		Th10Ai();
		~Th10Ai();

		void run();

	private:
		void start();
		void stop();

		bool handle();
		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();
		void move(DIR dir, bool slow);

		bool m_active;
		int64_t m_bombTime;
		int_t m_bombCount;

		interprocess::managed_windows_shared_memory m_sharedMemory;
		SharedData* m_sharedData;

		Status m_status2;
		Status m_status1;
		Status m_status;

		Scene m_scene;
	};
}

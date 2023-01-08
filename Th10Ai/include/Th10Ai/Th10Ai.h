#pragma once

#include "Th10Ai/Common.h"

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <Base/Time.h>

#include "Th10Base/SharedMemory.h"
#include "Th10Base/SharedData.h"
#include "Th10Base/Player.h"
#include "Th10Base/Item.h"
#include "Th10Base/Enemy.h"
#include "Th10Base/Bullet.h"
#include "Th10Base/Laser.h"
#include "Th10Ai/Status.h"
#include "Th10Ai/Scene.h"
//#include "Th10Ai/Input.h"

namespace th
{
	class Th10Ai
	{
	public:
		Th10Ai();
		~Th10Ai();

		void run();

	private:
		void controlProc();
		void start();
		void stop();

		bool handle();
		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();

		std::optional<Item> findItem();
		std::optional<Enemy> findEnemy();

		std::thread m_controlThread;
		std::atomic<bool> m_controlDone;
		std::thread m_handleThread;
		std::atomic<bool> m_handleDone;

		SharedMemory m_sharedMemory;
		SharedData* m_sharedData;

		//Status m_status2;
		//Status m_status1;
		//Status m_status0;
		Status m_status;

		//Scene m_scene2;
		//Scene m_scene1;
		Scene m_scene;

		Time m_bombTime;
		int_t m_bombCount;
		Time m_findItemTime;

		std::atomic<int_t> inputFrame;
		std::atomic<int_t> statusFrame;
		std::atomic<int_t> handleFrame;
	};
}

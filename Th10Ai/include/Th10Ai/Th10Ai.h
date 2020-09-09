#pragma once

#include "Th10Ai/Common.h"

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <boost/interprocess/managed_windows_shared_memory.hpp>

#include "Th10Ai/SharedData.h"
#include "Th10Ai/Status.h"
#include "Th10Ai/Scene.h"

namespace th
{
	namespace interprocess = boost::interprocess;

	class MyWindow;

	class Th10Ai
	{
	public:
		Th10Ai(MyWindow* myWindow);
		~Th10Ai();

		MyWindow* m_myWindow;

	private:
		static bool IsKeyPressed(int vKey);

		void controlProc();
		void start();
		void stop();

		void readProc();

		void handleProc();
		void handle();
		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();
		void move(DIR dir, bool slow);

		interprocess::managed_windows_shared_memory m_sharedMemory;
		SharedData* m_sharedData;

		std::thread m_controlThread;
		std::atomic_bool m_controlDone;
		std::atomic_bool m_controlActive;

		std::thread m_readThread;
		std::atomic_bool m_readDone;
		std::atomic_bool m_readActive;

		std::thread m_handleThread;
		std::atomic_bool m_handleDone;
		std::atomic_bool m_handleActive;
		int_t m_bombCount;
		DIR m_prevDir;
		bool m_prevSlow;

		Status m_status2;
		Status m_status1;
		Status m_status;
		//std::mutex m_statusMutex;
		//std::shared_ptr<Status> m_writeStatus;
		//std::shared_ptr<Status> m_middleStatus;
		//std::shared_ptr<Status> m_readStatus;
		//std::atomic_bool m_statusUpdated;
		Scene m_scene;

		int64_t m_bombTime;
	};
}

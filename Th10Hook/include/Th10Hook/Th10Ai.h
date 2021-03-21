#pragma once

#include "Th10Hook/Common.h"

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <boost/optional.hpp>

#include "Th10Hook/Console.h"
#include "Th10Hook/Status.h"
#include "Th10Hook/Scene.h"
#include "Th10Hook/Input.h"

namespace th
{
	class Th10Ai
	{
	public:
		Th10Ai(HWND window);
		~Th10Ai();

		void updateStatus();
		void commitInput(DWORD size, LPVOID data);

	private:
		void controlProc();
		void start();
		void stop();

		void handleProc();
		bool handle();
		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();

		boost::optional<Item> findItem();
		boost::optional<Enemy> findEnemy();

		Console m_console;

		std::thread m_controlThread;
		std::atomic<bool> m_controlDone;
		std::thread m_handleThread;
		std::atomic<bool> m_handleDone;
		std::atomic<bool> m_active;

		std::unique_ptr<Status> m_writeStatus;
		std::unique_ptr<Status> m_middleStatus;
		std::unique_ptr<Status> m_readStatus;
		std::mutex m_statusMutex;
		std::condition_variable m_statusCond;
		bool m_statusUpdated;
		Status m_status2;
		Status m_status1;
		Status m_status0;

		Scene m_scene;

		int64_t m_bombTime;
		int_t m_bombCount;

		int64_t m_findItemTime;

		Input m_input;
		std::atomic<bool> m_inputUpdated;

		uint_t statusFrame;
		uint_t inputFrame;
	};

	extern std::unique_ptr<Th10Ai> g_th10Ai;
}

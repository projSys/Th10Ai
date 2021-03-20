#pragma once

#include "Th10Hook/Common.h"

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Th10Hook/Console.h"
#include "Th10Hook/Th10Types.h"
#include "Th10Hook/Status.h"
#include "Th10Hook/Scene.h"

namespace th
{
	class Th10Ai
	{
	public:
		Th10Ai(HWND window);
		~Th10Ai();

		void updateStatus();
		void commitAction(DWORD size, LPVOID data);

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
		void move(DIR dir, bool slow);

		Console m_console;

		std::thread m_controlThread;
		std::atomic<bool> m_controlDone;
		std::thread m_handleThread;
		std::atomic<bool> m_handleDone;

		std::atomic<bool> m_active;
		int64_t m_bombTime;
		int_t m_bombCount;

		Status m_status2;
		Status m_status1;
		Status m_status0;
		Status m_status;
		std::mutex m_statusMutex;
		std::condition_variable m_statusCond;
		bool m_statusUpdated;

		Scene m_scene;

		ActionData m_actionData;
		std::atomic<bool> m_actionUpdated;
	};

	extern std::unique_ptr<Th10Ai> g_th10Ai;
}

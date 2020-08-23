#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

#include "Th10Ai/Th10Context.h"
#include "Th10Ai/Status.h"
#include "Th10Ai/Scene.h"

namespace th
{
	class MyWindow;

	class Th10Ai
	{
	public:
		Th10Ai(MyWindow* window);
		~Th10Ai();

		MyWindow* m_window;

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
		void move(Direction dir, bool slow);

		Th10Context m_context;

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
		Direction m_prevDir;
		bool m_prevSlow;

		Status m_status;
		//std::mutex m_statusMutex;
		//std::shared_ptr<Status> m_writeStatus;
		//std::shared_ptr<Status> m_middleStatus;
		//std::shared_ptr<Status> m_readStatus;
		//std::atomic_bool m_statusUpdated;
		Scene m_scene;
	};
}

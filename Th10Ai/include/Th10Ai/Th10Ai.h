#pragma once

#include "Th10Ai/Common.h"

#include <thread>
#include <atomic>
#include <optional>
#include <Base/Time.h>

#include "Th10Ai/Config.h"
#include "Th10Ai/Th10Hook.h"
#include "Th10Ai/Status.h"
#include "Th10Ai/Scene.h"

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

	private:
		Config m_config;
		Th10Hook m_th10Hook;

		std::thread m_controlThread;
		std::atomic<bool> m_controlDone;
		std::thread m_handleThread;
		std::atomic<bool> m_handleDone;

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
		Time m_handleTime;
	};
}

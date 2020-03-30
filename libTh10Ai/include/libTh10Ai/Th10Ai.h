#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

#include "libTh10Ai/D3D9Hook.h"
#include "libTh10Ai/DI8Hook.h"
#include "libTh10Ai/Reader.h"
#include "libTh10Ai/Data.h"
#include "libTh10Ai/Scene.h"

namespace th
{
	class Th10Ai
	{
	public:
		Th10Ai();
		~Th10Ai();

	private:
		static bool IsKeyPressed(int vKey);

		void controlProc();
		void start();
		void stop();
		void print();

		void readProc();

		void handleProc();
		void handle();
		bool handleBomb();
		bool handleTalk();
		bool handleShoot();
		bool handleMove();
		void move(Direction dir, bool slow);

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

		D3D9Hook m_d3d9Hook;
		DI8Hook m_di8Hook;
		Reader m_reader;
		//Data m_data;
		std::mutex m_dataMutex;
		std::shared_ptr<Data> m_writeData;
		std::shared_ptr<Data> m_middleData;
		std::shared_ptr<Data> m_readData;
		std::atomic_bool m_dataUpdated;
		Scene m_scene;

		bool p;
	};
}

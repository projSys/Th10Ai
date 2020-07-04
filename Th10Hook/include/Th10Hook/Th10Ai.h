#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

#include "Th10Hook/D3D9Hook.h"
#include "Th10Hook/DI8Hook.h"
#include "Th10Hook/Reader.h"
#include "Th10Hook/Data.h"
#include "Th10Hook/Scene.h"

namespace th
{
	class Th10Ai
	{
	public:
		Th10Ai();
		~Th10Ai();

		void controlProc();

	private:
		static bool IsKeyPressed(int vKey);

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

		thread m_controlThread;
		atomic_bool m_controlDone;
		atomic_bool m_controlActive;

		thread m_readThread;
		atomic_bool m_readDone;
		atomic_bool m_readActive;

		thread m_handleThread;
		atomic_bool m_handleDone;
		atomic_bool m_handleActive;
		int_t m_bombCount;
		Direction m_prevDir;
		bool m_prevSlow;

		D3D9Hook m_d3d9Hook;
		DI8Hook m_di8Hook;
		Reader m_reader;
		//Data m_data;
		mutex m_dataMutex;
		shared_ptr<Data> m_writeData;
		shared_ptr<Data> m_middleData;
		shared_ptr<Data> m_readData;
		atomic_bool m_dataUpdated;
		Scene m_scene;

		bool p;
	};
}

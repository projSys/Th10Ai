#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

#include "Th10Ai/Th10Context.h"
#include "Th10Ai/Data.h"
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

		Data m_data;
		//mutex m_dataMutex;
		//shared_ptr<Data> m_writeData;
		//shared_ptr<Data> m_middleData;
		//shared_ptr<Data> m_readData;
		//atomic_bool m_dataUpdated;
		Scene m_scene;
	};
}

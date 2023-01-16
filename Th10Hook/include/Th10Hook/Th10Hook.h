#pragma once

#include "Th10Hook/Common.h"

#include <memory>
#include <Base/Time.h>
#include <Th10Base/SharedMemory.h>
#include <Th10Base/SharedData.h>

//#include "Th10Hook/Console.h"

namespace th
{
	class Th10Hook
	{
	public:
		explicit Th10Hook(HWND window);
		~Th10Hook();

		void updateStatus();
		void commitInputTo(DWORD size, LPVOID data);

	private:
		void notifyInit();

		void notifyExit();
		void notifyUpdate();

		bool waitInput(const Time& timeout = Time(8));

	private:
		SharedMemory m_sharedMemory;
		SharedData* m_sharedData;

		//Console m_console;
	};

	extern std::unique_ptr<Th10Hook> g_th10Hook;
}

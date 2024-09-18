#pragma once

#include "Th10Hook/Common.h"

#include <memory>

#include "Th10Base/SharedMemory.h"
#include "Th10Base/SharedData.h"

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
		SharedMemory m_sharedMemory;
		SharedData* m_sharedData;
	};

	extern std::unique_ptr<Th10Hook> g_th10Hook;
}

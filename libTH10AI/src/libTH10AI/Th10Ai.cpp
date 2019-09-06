#include "libTH10AI/Common.h"
#include "libTH10AI/Th10Ai.h"

#include <thread>
#include <chrono>

#include "libTH10AI/HookThread.h"

namespace th
{
	Th10Ai::Th10Ai()
	{
	}

	Th10Ai::~Th10Ai()
	{
	}

	void Th10Ai::run(HookThread& container)
	{
		while (!container.isDone())
		{
			if (GetAsyncKeyState('D') & 0x8000)
				break;
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	}
}

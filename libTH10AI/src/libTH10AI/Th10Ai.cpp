#include "libTH10AI/Common.h"
#include "libTH10AI/Th10Ai.h"

namespace th
{
	Th10Ai::Th10Ai() :
		m_done(false)
	{
		m_thread = std::thread(std::bind(&Th10Ai::proc, this));
	}

	Th10Ai::~Th10Ai()
	{
		m_done = true;
		if (m_thread.joinable())
			m_thread.join();
	}

	void Th10Ai::proc()
	{
		while (!m_done)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	}
}

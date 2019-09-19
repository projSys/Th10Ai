#include "libTH10Bot/Common.h"
#include "libTH10Bot/HookThread.h"

#include "libTH10Bot/HookMain.h"
#include "libTH10Bot/Bot.h"

namespace th
{
	HookThread::HookThread() :
		m_done(false)
	{
		m_bot = std::make_shared<Bot>();
		m_thread = std::thread(std::bind(&HookThread::hookProc, this));
	}

	HookThread::~HookThread()
	{
		m_done = true;
		m_bot->notify();
		if (m_thread.joinable())
			m_thread.join();
	}

	void HookThread::hookProc()
	{
		try
		{
			m_bot->run(*this);
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}

		// 按D自行退出，m_done是false，需要destroy
		// 跟随WM_CLOSE退出，m_done是true，不需要destroy
		HookExit(!m_done);
	}

	bool HookThread::isDone() const
	{
		return m_done;
	}
}

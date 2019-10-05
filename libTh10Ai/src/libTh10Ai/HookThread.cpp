#include "libTh10Ai/Common.h"
#include "libTh10Ai/HookThread.h"

#include "libTh10Ai/HookMain.h"
#include "libTh10Ai/Ai.h"

namespace th
{
	HookThread::HookThread() :
		m_done(false)
	{
		m_ai = std::make_shared<Ai>();
		m_thread = std::thread(std::bind(&HookThread::hookProc, this));
	}

	HookThread::~HookThread()
	{
		m_done = true;
		m_ai->notify();
		if (m_thread.joinable())
			m_thread.join();
	}

	void HookThread::hookProc()
	{
		try
		{
			m_ai->run(*this);
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}

		// ��D�����˳���m_done��false����Ҫdestroy
		// ����WM_CLOSE�˳���m_done��true������Ҫdestroy
		HookExit(!m_done);
	}

	bool HookThread::isDone() const
	{
		return m_done;
	}
}

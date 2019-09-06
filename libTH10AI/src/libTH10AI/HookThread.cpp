#include "libTH10AI/Common.h"
#include "libTH10AI/HookThread.h"

#include "libTH10AI/HookMain.h"

namespace th
{
	HookThread::HookThread() :
		m_done(false)
	{
		m_thread = std::thread(std::bind(&HookThread::hookProc, this));
	}

	HookThread::~HookThread()
	{
		m_done = true;
		if (m_thread.joinable())
			m_thread.join();
	}

	void HookThread::hookProc()
	{
		try
		{
			std::shared_ptr<Th10Ai> ai = std::make_shared<Th10Ai>();
			ai->run(*this);
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

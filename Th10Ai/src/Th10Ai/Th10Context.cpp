#include "Th10Ai/Common.h"
#include "Th10Ai/Th10Context.h"

#include <Base/ScopeGuard.h>

#include "Th10Ai/DllInject.h"

namespace th
{
	using namespace boost::interprocess;
	using namespace boost::posix_time;

	Th10Context::Th10Context() :
		m_isDone(false),
		m_data(nullptr)
	{
		m_thread = thread(&Th10Context::threadProc, this);
	}

	Th10Context::~Th10Context()
	{
		if (m_thread.joinable())
			m_thread.join();
	}

	void Th10Context::threadProc()
	{
		try
		{
			HWND window = FindWindowW(L"BASE", nullptr);
			if (window == nullptr)
				BASE_THROW(Exception(u8"东方风神录未运行。"));

			DWORD processId = 0;
			DWORD threadId = GetWindowThreadProcessId(window, &processId);

			m_memory = managed_windows_shared_memory(create_only, "Th10SharedMemory", 8 * 1024 * 1024);
			//ON_SCOPE_EXIT([&]()
			//	{
			//		m_memory = managed_windows_shared_memory();
			//	});
			m_data = m_memory.construct<Th10SharedData>("Th10SharedData")();
			if (m_data == nullptr)
				BASE_THROW(Exception(u8"Th10SharedData名称已被使用。"));
			//ON_SCOPE_EXIT([&]()
			//	{
			//		m_memory.destroy_ptr(m_data);
			//	});

			m_data->window = window;
			m_data->isHooked = false;
			m_data->isUnhook = false;
			m_data->isUpdated = false;

			string dllName = Utils::GetModuleDir() + "/Th10Hook.dll";
			DllInject::EnableDebugPrivilege();
			DllInject::Inject(processId, dllName);

			{
				scoped_lock<interprocess_mutex> lock(m_data->hookMutex);
				if (!m_data->isHooked)
				{
					ptime absTime = microsec_clock::universal_time() + milliseconds(3000);
					if (!m_data->hookCond.timed_wait(lock, absTime))
						BASE_THROW(Exception(u8"Th10Hook初始化失败，详细信息请查看Th10Hook.log。"));
				}
			}
			//{
			//	scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
			//	m_data->isUnhook = true;
			//	m_data->unhookCond.notify_one();
			//}

			//while (!m_isDone)
			//{
			//	{
			//		scoped_lock<interprocess_mutex> lock(m_data->updateMutex);
			//		if (!m_data->isUpdated)
			//			m_data->updateCond.wait(lock);
			//		m_data->isUpdated = false;
			//	}


			//}
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}
	}
}

#include "Th10Ai/Common.h"
#include "Th10Ai/Th10Context.h"

#include <Base/ScopeGuard.h>

#include "Th10Ai/DllInject.h"

namespace th
{
	using namespace boost::interprocess;
	using namespace boost::posix_time;

	Th10Context::Th10Context() :
		m_data(nullptr)
	{
		HWND window = FindWindowW(L"BASE", nullptr);
		if (window == nullptr)
			BASE_THROW(Exception(u8"东方风神录未运行。"));

		DWORD processId = 0;
		DWORD threadId = GetWindowThreadProcessId(window, &processId);

		m_memory = managed_windows_shared_memory(create_only, "Th10SharedMemory", 8 * 1024 * 1024);
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
		m_data->updateTime = 0;

		string dllName = Apis::GetModuleDir() + "/Th10Hook.dll";
		DllInject::EnableDebugPrivilege();
		DllInject::Inject(processId, dllName);

		if (!timedWaitHook(3000))
			BASE_THROW(Exception(u8"Th10Hook初始化失败，详细信息请查看Th10Hook.log。"));
	}

	Th10Context::~Th10Context()
	{
	}

	void Th10Context::activate()
	{
		if (!SetForegroundWindow(m_data->window))
			BASE_THROW(WindowsError());
	}

	bool Th10Context::timedWaitHook(time_t ms)
	{
		bool isNotTimeout = true;
		scoped_lock<interprocess_mutex> lock(m_data->hookMutex);
		if (!m_data->isHooked)
		{
			ptime absTime = microsec_clock::universal_time() + milliseconds(ms);
			isNotTimeout = m_data->hookCond.timed_wait(lock, absTime);
		}
		return isNotTimeout;
	}

	void Th10Context::notifyUnhook()
	{
		scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
		m_data->isUnhook = true;
		m_data->unhookCond.notify_one();
	}

	void Th10Context::notifyUpdate()
	{
		scoped_lock<interprocess_mutex> lock(m_data->updateMutex);
		m_data->isUpdated = true;
		m_data->updateCond.notify_one();
	}

	void Th10Context::waitUpdate()
	{
		scoped_lock<interprocess_mutex> lock(m_data->updateMutex);
		if (!m_data->isUpdated)
			m_data->updateCond.wait(lock);
		m_data->isUpdated = false;

		cout << m_clock.update() - m_data->updateTime << endl;
	}

	Status_t& Th10Context::getStatus()
	{
		return m_data->status;
	}
}

#include "Th10Ai/SharedMemory.h"

#include <Base/ScopeGuard.h>
#include <Base/Clock.h>

#include "Th10Ai/DllInject.h"

namespace th
{
	namespace posix_time = boost::posix_time;

	SharedMemory::SharedMemory() :
		m_data(nullptr)
	{
		HWND window = FindWindowW(L"BASE", L"搶曽晽恄榐丂乣 Mountain of Faith. ver 1.00a");
		if (window == nullptr)
			BASE_THROW(Exception(u8"东方风神录未运行。"));
		DWORD processId = 0;
		DWORD threadId = GetWindowThreadProcessId(window, &processId);

		m_memory = interprocess::managed_windows_shared_memory(interprocess::create_only,
			"Th10-SharedMemory", 8 * 1024 * 1024);
		m_data = m_memory.construct<SharedData>("Th10-SharedData")();
		if (m_data == nullptr)
			BASE_THROW(Exception(u8"Th10-SharedData名称已被使用。"));

		m_data->window = window;
		m_data->hooked = false;
		m_data->unhooked = false;
		m_data->updated = false;
		m_data->exited = false;
		m_data->updateTime = 0;
		//m_data->status = {};
		//m_data->action = {};
		m_data->actionUpdated = false;
		memset(&m_data->status, 0, sizeof(StatusData));
		memset(&m_data->action, 0, sizeof(ActionData));

		std::string dllName = Apis::GetModuleDir() + "/Th10Hook.dll";
		DllInject::EnableDebugPrivilege();
		DllInject::Inject(processId, dllName);

		if (!timedWaitHook(3000))
			BASE_THROW(Exception(u8"Th10Hook初始化失败，详细信息请查看Th10Hook.log。"));
	}

	SharedMemory::~SharedMemory()
	{
		if (m_data != nullptr)
			m_memory.destroy_ptr(m_data);
	}

	void SharedMemory::activate()
	{
		if (!SetForegroundWindow(m_data->window))
			BASE_THROW(WindowsError());
	}

	bool SharedMemory::timedWaitHook(int64_t ms)
	{
		bool notTimeout = true;
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->hookMutex);
		if (!m_data->hooked)
		{
			posix_time::ptime absTime = interprocess::microsec_clock::universal_time()
				+ posix_time::milliseconds(ms);
			notTimeout = m_data->hookCond.timed_wait(lock, absTime);
		}
		return notTimeout;
	}

	void SharedMemory::notifyUnhook()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->unhookMutex);
		m_data->unhooked = true;
		m_data->unhookCond.notify_one();
	}

	bool SharedMemory::waitUpdate()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->updateMutex);

		if (m_data->updated)
			std::cout << "读取太慢了。" << std::endl;

		if (!m_data->updated && !m_data->exited)
			m_data->updateCond.wait(lock);

		m_data->updated = false;

		//Clock clock;
		//clock.update();
		//std::cout << clock.getTime() - m_data->updateTime << std::endl;

		return !m_data->exited;
	}

	const StatusData& SharedMemory::getStatus() const
	{
		return m_data->status;
	}

	ActionData& SharedMemory::getAction()
	{
		return m_data->action;
	}

	void SharedMemory::commit()
	{
		m_data->actionUpdated = true;
	}
}

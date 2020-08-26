#include "Th10Ai/Th10Context.h"

#include <Base/ScopeGuard.h>
#include <Base/Clock.h>
#include <Windows/WindowsError.h>
#include <Windows/Apis.h>

#include "Th10Ai/DllInject.h"

namespace th
{
	namespace posix_time = boost::posix_time;

	Th10Context::Th10Context() :
		m_data(nullptr)
	{
		HWND window = FindWindowW(L"BASE", L"�������_�^�@�` Mountain of Faith. ver 1.00a");
		if (window == nullptr)
			BASE_THROW(Exception(u8"��������¼δ���С�"));
		DWORD processId = 0;
		DWORD threadId = GetWindowThreadProcessId(window, &processId);

		m_memory = interprocess::managed_windows_shared_memory(interprocess::create_only,
			"Th10SharedMemory", 8 * 1024 * 1024);
		m_data = m_memory.construct<Th10SharedData>("Th10SharedData")();
		if (m_data == nullptr)
			BASE_THROW(Exception(u8"Th10SharedData�����ѱ�ʹ�á�"));

		m_data->window = window;
		m_data->hooked = false;
		m_data->unhooked = false;
		m_data->updated = false;
		m_data->exited = false;
		m_data->updateTime = 0;
		//m_data->status = {};
		//m_data->action = {};
		memset(&m_data->status, 0, sizeof(StatusData));
		memset(&m_data->action, 0, sizeof(ActionData));

		std::string dllName = Apis::GetModuleDir() + "/Th10Hook.dll";
		DllInject::EnableDebugPrivilege();
		DllInject::Inject(processId, dllName);

		if (!timedWaitHook(3000))
			BASE_THROW(Exception(u8"Th10Hook��ʼ��ʧ�ܣ���ϸ��Ϣ��鿴Th10Hook.log��"));
	}

	Th10Context::~Th10Context()
	{
		if (m_data != nullptr)
			m_memory.destroy_ptr(m_data);
	}

	void Th10Context::activate()
	{
		if (!SetForegroundWindow(m_data->window))
			BASE_THROW(WindowsError());
	}

	bool Th10Context::timedWaitHook(int64_t ms)
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

	void Th10Context::notifyUnhook()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->unhookMutex);
		m_data->unhooked = true;
		m_data->unhookCond.notify_one();
	}

	bool Th10Context::waitUpdate()
	{
		interprocess::scoped_lock<interprocess::interprocess_mutex> lock(m_data->updateMutex);
		if (!m_data->updated && !m_data->exited)
			m_data->updateCond.wait(lock);
		m_data->updated = false;

		//Clock clock;
		//clock.update();
		//std::cout << clock.getTime() - m_data->updateTime << std::endl;

		return !m_data->exited;
	}

	const StatusData& Th10Context::getStatus() const
	{
		return m_data->status;
	}
}

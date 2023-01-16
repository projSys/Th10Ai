#include "Th10Ai/Th10Hook.h"

#include <boost/process/detail/handler_base.hpp>
#include <Base/Error.h>
#include <Base/Catcher.h>
#include <Base/Windows/Apis.h>

#include "Th10Ai/DllInject.h"

namespace th
{
	Th10Hook::Th10Hook() :
		m_sharedMemory(bip::create_only, "Th10SharedMemory", 64 * 1024 * 1024),
		m_sharedData(nullptr)
	{
		m_sharedData = m_sharedMemory.construct<SharedData>("Th10SharedData")(m_sharedMemory);
		if (m_sharedData == nullptr)
			throw Exception("Th10SharedData名称已被使用。");
	}

	Th10Hook::~Th10Hook()
	{
		if (m_stdioThread.joinable())
			m_stdioThread.join();

		m_sharedMemory.destroy_ptr(m_sharedData);
	}

	class LaunchHandler :
		public bp::detail::handler_base
	{
	public:
		template <typename Executor>
		void on_setup(Executor& exec) const
		{
			exec.creation_flags |= CREATE_SUSPENDED;
		}

		template <typename Executor>
		void on_error(Executor& exec, const std::error_code& ec) const
		{
			throw Error(ec.value());
		}

		template <typename Executor>
		void on_success(Executor& exec) const
		{
			fs::path dir = Apis::GetModuleDir();
			fs::path dllPath = dir / L"Th10Hook.dll";

			//DllInject::EnableDebugPrivilege();
			DllInject::Inject(exec.proc_info.hProcess, dllPath);

			DWORD count = ResumeThread(exec.proc_info.hThread);
			if (count == (DWORD)-1)
				throw Error(GetLastError());
		}
	};

	void Th10Hook::launch(const fs::path& exePath)
	{
		fs::path exeDir = exePath.parent_path();
		m_th10 = bp::child(exePath.c_str(),
			bp::std_out > m_ips, //bp::std_err > stderr, bp::std_in < stdin,
			bp::start_dir(exeDir.c_str()), LaunchHandler());
		m_stdioThread = std::thread(&Th10Hook::stdioProc, this);

		if (!waitInit())
			throw Exception("Th10Hook初始化超时。");
	}

	void Th10Hook::stdioProc()
	{
		try
		{
			std::string line;
			while (m_th10.running() && std::getline(m_ips, line) && !line.empty())
				std::cout << line.c_str() << std::endl;
		}
		catch (...)
		{
			BASE_LOG(error) << Catcher() << std::endl;
			ExitProcess(1);
		}
	}

	bool Th10Hook::waitInit(const Time& timeout)
	{
		//bip::scoped_lock<bip::interprocess_mutex> lock(m_sharedData->initMutex);
		bip::scoped_lock<bip::interprocess_mutex> lock(m_sharedData->statusMutex);
		while (!m_sharedData->inited)
		{
			//bip::cv_status status = m_sharedData->initCond.wait_for(lock, timeout);
			bip::cv_status status = m_sharedData->statusCond.wait_for(lock, timeout);
			if (status == bip::cv_status::timeout)
				return false;
		}
		return true;
	}

	bool Th10Hook::isActive() const
	{
		return m_sharedData->active;
	}

	void Th10Hook::setActive(bool active)
	{
		m_sharedData->active = active;
	}

	HWND Th10Hook::getWindow() const
	{
		return m_sharedData->window;
	}

	bool Th10Hook::waitUpdate()
	{
		bip::scoped_lock<bip::interprocess_mutex> lock(m_sharedData->statusMutex);
		if (m_sharedData->statusUpdated)
			std::cout << "警告：处理太慢，状态已更新。" << std::endl;
		while (!m_sharedData->statusUpdated && !m_sharedData->exit)
			m_sharedData->statusCond.wait(lock);
		m_sharedData->readableStatus.swap(m_sharedData->swappableStatus);
		m_sharedData->statusUpdated = false;
		return !m_sharedData->exit;
	}

	const SharedStatus& Th10Hook::getReadableStatus() const
	{
		return *(m_sharedData->readableStatus);
	}

	void Th10Hook::notifyInput()
	{
		{
			bip::scoped_lock<bip::interprocess_mutex> lock(m_sharedData->inputMutex);
			if (m_sharedData->inputUpdated)
				std::cout << "错误：处理太慢，输入跳帧了。" << std::endl;
			m_sharedData->writableInput.swap(m_sharedData->swappableInput);
			m_sharedData->inputUpdated = true;
		}
		m_sharedData->inputCond.notify_one();
	}

	SharedInput& Th10Hook::getWritableInput()
	{
		return *(m_sharedData->writableInput);
	}
}

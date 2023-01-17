#include "Th10Ai/Th10Hook.h"

#include <boost/process/detail/handler_base.hpp>
#include <Base/Error.h>
#include <Base/Catcher.h>
#include <Base/Windows/Apis.h>

#include "Th10Ai/Config.h"
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
		if (m_th10Thread.joinable())
			m_th10Thread.join();

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

	void Th10Hook::launch(const Config& config)
	{
		fs::path th10Dir = config.th10Path.parent_path();
		m_th10 = bp::child(config.th10Path.c_str(),
			bp::std_out > m_ips, //bp::std_err > stderr, bp::std_in < stdin,
			bp::start_dir(th10Dir.c_str()), LaunchHandler());
		m_th10Thread = std::thread(&Th10Hook::th10Proc, this);

		if (!m_sharedData->waitInit(Time(3000)))
			throw Exception("Th10Hook初始化超时。");
	}

	void Th10Hook::th10Proc()
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

	bool Th10Hook::isActive() const
	{
		return m_sharedData->isActive();
	}

	void Th10Hook::setActive(bool active)
	{
		m_sharedData->setActive(active);
	}

	HWND Th10Hook::getWindow() const
	{
		return m_sharedData->getWindow();
	}

	bool Th10Hook::waitUpdate()
	{
		return m_sharedData->waitUpdate();
	}

	bool Th10Hook::waitUpdate(const Time& timeout)
	{
		return m_sharedData->waitUpdate(timeout);
	}

	bool Th10Hook::isExit() const
	{
		return m_sharedData->isExit();
	}

	const SharedStatus& Th10Hook::getReadableStatus() const
	{
		return m_sharedData->getReadableStatus();
	}

	void Th10Hook::notifyInput()
	{
		m_sharedData->notifyInput();
	}

	SharedInput& Th10Hook::getWritableInput()
	{
		return m_sharedData->getWritableInput();
	}
}

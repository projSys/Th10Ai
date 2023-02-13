#include "Th10Ai/Th10Hook.h"

#include <boost/process/detail/handler_base.hpp>
#include <boost/locale.hpp>
#include <Base/Exception/Exception.h>
#include <Base/Exception/SystemError.h>
#include <Base/Exception/Catcher.h>
#include <Base/Windows/Apis.h>

#include "Th10Ai/Config.h"
#include "Th10Ai/DllInject.h"

namespace th
{
	namespace bs = boost::system;
	namespace bloc = boost::locale;
	namespace blc = boost::locale::conv;
	namespace blu = boost::locale::util;

	Th10Hook::Th10Hook() :
		m_sharedMemory(bip::create_only, "Th10SharedMemory", MEMORY_SIZE),
		m_sharedData(nullptr),
		m_apOut(m_ioc),
		m_apErr(m_ioc),
		m_bufOut{},
		m_bufErr{}
	{
		m_sharedData = m_sharedMemory.construct<SharedData>("Th10SharedData")(m_sharedMemory);
		if (m_sharedData == nullptr)
			Throw(Exception("Th10SharedData构造失败。"));
	}

	Th10Hook::~Th10Hook()
	{
		m_ioc.stop();
		if (m_asioThread.joinable())
			m_asioThread.join();

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
			Throw(SystemError(ec));
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
				Throw(SystemError(GetLastError()));
		}
	};

	void Th10Hook::launch(const Config& config)
	{
		fs::path th10Dir = config.th10Path.parent_path();
		m_th10 = bp::child(config.th10Path.native(),
			bp::std_out > m_apOut, bp::std_err > m_apErr, //bp::std_in < stdin,
			bp::start_dir(th10Dir.native()), LaunchHandler());
		m_asioThread = std::thread(&Th10Hook::asioProc, this);

		if (!m_sharedData->waitInit(Time(3000)))
			Throw(Exception("Th10Hook初始化超时。"));
	}

	void Th10Hook::asioProc()
	{
		try
		{
			doStdoutRead();
			doStderrRead();
			m_ioc.run();
		}
		catch (...)
		{
			LOG_FATAL() << Catcher() << std::endl;
			Log::Flush();
			//ExitProcess(1);
		}
	}

	void Th10Hook::doStdoutRead()
	{
		m_apOut.async_read_some(
			ba::buffer(m_bufOut, BUFFER_SIZE - 1),
			[this](const bs::error_code& ec, uint_t size)
			{
				if (!ec)
				{
					m_bufOut[size] = '\0';
					std::cout << m_bufOut;
					doStdoutRead();
				}
				else
				{
					std::string msg = ec.message();
					std::string id = blu::get_system_locale();
					if (id.find("UTF-8") == std::string::npos)
					{
						bloc::generator gen;
						msg = blc::to_utf<char>(msg, gen(id));
					}
					std::cout << msg << std::endl;
				}
			});
	}

	void Th10Hook::doStderrRead()
	{
		m_apErr.async_read_some(
			ba::buffer(m_bufErr, BUFFER_SIZE - 1),
			[this](const bs::error_code& ec, uint_t size)
			{
				if (!ec)
				{
					m_bufErr[size] = '\0';
					std::cerr << m_bufErr;
					doStderrRead();
				}
				else
				{
					std::string msg = ec.message();
					std::string id = blu::get_system_locale();
					if (id.find("UTF-8") == std::string::npos)
					{
						bloc::generator gen;
						msg = blc::to_utf<char>(msg, gen(id));
					}
					std::cout << msg << std::endl;
				}
			});
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

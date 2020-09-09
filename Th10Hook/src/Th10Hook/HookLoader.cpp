#include "Th10Hook/HookLoader.h"

#include <memory>

#include "Th10Hook/HookMain.h"
#include "Th10Hook/DllMain.h"
#include "Th10Hook/WindowHook.h"
#include "Th10Hook/SelfFree.h"

namespace th
{
	HookLoader g_hookLoader;

	HookLoader::HookLoader() :
		Singleton(this)
	{
	}

	void HookLoader::start()
	{
		m_thread = std::thread(&HookLoader::proc, this);
	}

	void HookLoader::join()
	{
		if (m_thread.joinable())
			m_thread.join();
	}

	void HookLoader::proc()
	{
		try
		{
			std::string logName = Apis::GetModuleDir(g_dll) + "/Th10Hook_%N.log";
			m_logger.addFileLog(logName);

			std::unique_ptr<HookMain> hookMain = std::make_unique<HookMain>();
			hookMain->run();
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}

		WindowHook& windowHook = WindowHook::GetInstance();
		if (!windowHook.isDestroyed())
			SelfFree::Free();
	}
}

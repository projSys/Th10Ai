#include "Th10Hook/DllLoader.h"

#include <memory>
#include <Base/Windows/Module.h>

#include "Th10Hook/DllMain.h"
#include "Th10Hook/DllFree.h"
#include "Th10Hook/WindowHook.h"
#include "Th10Hook/Th10Ai.h"
#include "Th10Hook/Console.h"

namespace th
{
	DllLoader g_dllLoader;

	DllLoader::DllLoader() :
		Singleton(this)
	{
	}

	void DllLoader::start()
	{
		m_thread = boost::thread(&DllLoader::proc, this);
	}

	void DllLoader::join()
	{
		if (m_thread.joinable())
			m_thread.join();
	}

	void DllLoader::proc()
	{
		try
		{
			Console console;

			//std::string logPath = Module(g_module).getDir() + "/Th10Hook_%N.log";
			//m_logger.addFileLog(logPath);

			std::unique_ptr<Th10Ai> th10Ai = std::make_unique<Th10Ai>();
			th10Ai->run();
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}

		WindowHook& windowHook = WindowHook::GetInstance();
		if (!windowHook.isDestroyed())
			DllFree::SelfFree();
	}
}

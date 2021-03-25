#include "Th10Hook/DllMain.h"

#include <Base/Windows/Module.h>

#include "Th10Hook/MyDetours.h"
#include "Th10Hook/DirectX/D3D9Hook.h"
#include "Th10Hook/DirectX/DI8Hook.h"

HMODULE g_module = nullptr;
base::Logger g_logger;
th::D3D9Hook g_d3d9Hook;
th::DI8Hook g_di8Hook;

void Hook()
{
	try
	{
		std::string logPath = base::win::Module(g_module).getDir() + "/Th10Hook_%N.log";
		g_logger.addFileLog(logPath);

		th::MyDetours::TransactionBegin();
		g_d3d9Hook.attach();
		g_di8Hook.attach();
		th::MyDetours::TransactionCommit();
	}
	catch (...)
	{
		BASE_LOG_ERROR(base::PrintException());
	}
}

void Unhook()
{
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reasonForCall, LPVOID reserved)
{
	switch (reasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		g_module = module;
		Hook();
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		Unhook();
		break;
	}
	return TRUE;
}

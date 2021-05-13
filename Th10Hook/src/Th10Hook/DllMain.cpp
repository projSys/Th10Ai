#include "Th10Hook/DllMain.h"

#include <memory>
#include <boost/filesystem.hpp>
#include <Base/Windows/Module.h>

#include "Th10Hook/MyDetours.h"
#include "Th10Hook/DirectX/D3D9Hook.h"
#include "Th10Hook/DirectX/DI8Hook.h"

using namespace base;
using base::float_t;
using namespace base::win;
using namespace th;

HMODULE g_module = nullptr;
Logger g_logger;
std::unique_ptr<D3D9Hook> g_d3d9Hook;
std::unique_ptr<DI8Hook> g_di8Hook;

void Hook()
{
	MyDetours detours;
	try
	{
		fs::path logPath = Module(g_module).getDir() / L"Th10Hook_%N.log";
		g_logger.addFileLog(logPath);

		g_d3d9Hook = std::make_unique<D3D9Hook>();
		g_di8Hook = std::make_unique<DI8Hook>();

		detours.transactionBegin();
		g_d3d9Hook->attach(detours);
		g_di8Hook->attach(detours);
		detours.transactionCommit();
	}
	catch (...)
	{
		detours.transactionAbort();
		BASE_LOG_ERROR(PrintException());
	}
}

void Unhook()
{
	MyDetours detours;
	try
	{
		detours.transactionBegin();
		g_d3d9Hook->detach(detours);
		g_di8Hook->detach(detours);
		detours.transactionCommit();

		g_d3d9Hook = nullptr;
		g_di8Hook = nullptr;
	}
	catch (...)
	{
		detours.transactionAbort();
		BASE_LOG_ERROR(PrintException());
	}
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
		//Unhook();
		break;
	}
	return TRUE;
}

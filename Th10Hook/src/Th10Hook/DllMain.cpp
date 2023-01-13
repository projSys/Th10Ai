#include "Th10Hook/DllMain.h"

#include <Base/Catcher.h>
#include <Base/Logger.h>
#include <Base/Windows/Apis.h>

#include "Th10Hook/MyDetours.h"
#include "Th10Hook/D3d9Hook.h"
#include "Th10Hook/Di8Hook.h"

using namespace th;

HMODULE g_module = nullptr;

void Hook()
{
	MyDetours detours;
	try
	{
		fs::path logPath = Apis::GetModuleDir(g_module) / L"Th10Hook.log";
		g_logger.addFileLog(logPath);
		g_logger.addCommonAttributes();

		detours.transactionBegin();
		g_d3d9Hook.attach(detours);
		g_di8Hook.attach(detours);
		detours.transactionCommit();
	}
	catch (...)
	{
		detours.transactionAbort();
		BASE_LOG(error) << Catcher() << std::endl;
		ExitProcess(1);
	}
}

void Unhook()
{
	MyDetours detours;
	try
	{
		detours.transactionBegin();
		g_d3d9Hook.detach(detours);
		g_di8Hook.detach(detours);
		detours.transactionCommit();
	}
	catch (...)
	{
		detours.transactionAbort();
		BASE_LOG(error) << Catcher() << std::endl;
		ExitProcess(1);
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

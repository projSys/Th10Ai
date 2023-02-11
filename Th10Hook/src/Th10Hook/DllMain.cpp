#include "Th10Hook/DllMain.h"

#include <Base/Exception/Catcher.h>
#include <Base/Windows/Apis.h>

#include "Th10Hook/MyDetours.h"
#include "Th10Hook/D3d9Hook.h"
#include "Th10Hook/Di8Hook.h"

using namespace th;

HMODULE g_module = nullptr;

void Hook()
{
	try
	{
		fs::path dir = Apis::GetModuleDir();
		fs::path logPath = dir / L"Th10Hook.log";
		Log::Initialize(logPath);
	}
	catch (...)
	{
		MessageBoxW(nullptr, L"Log初始化失败。", L"Th10Hook", MB_OK);
		throw;
	}

	MyDetours detours;
	try
	{
		detours.transactionBegin();
		g_d3d9Hook.attach(detours);
		g_di8Hook.attach(detours);
		detours.transactionCommit();
	}
	catch (...)
	{
		detours.transactionAbort();
		LOG_FATAL() << Catcher() << std::endl;
		Log::Flush();
		//ExitProcess(1);
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
		LOG_FATAL() << Catcher() << std::endl;
		Log::Flush();
		//ExitProcess(1);
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

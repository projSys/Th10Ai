#include "Th10Hook/DllMain.h"

#include "Th10Hook/HookLoader.h"

HMODULE g_dll = nullptr;

BOOL APIENTRY DllMain(HMODULE module, DWORD reasonForCall, LPVOID reserved)
{
	switch (reasonForCall)
	{
	case DLL_PROCESS_ATTACH:
	{
		g_dll = module;
		th::HookLoader& hookLoader = th::HookLoader::GetInstance();
		hookLoader.start();
		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

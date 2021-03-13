#include "Th10Hook/DllMain.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <d3d9.h>
#include <dinput.h>
#include <detours.h>
#include <Base/Windows/Module.h>

#include "Th10Hook/DllLoader.h"
#include "Th10Hook/MyDetours.h"
#include "Th10Hook/DirectX/MyDirect3D9.h"
#include "Th10Hook/DirectX/MyDirectInput8A.h"

HMODULE g_module = nullptr;

base::Logger logger;

using Direct3DCreate9_t = decltype(&Direct3DCreate9);
Direct3DCreate9_t direct3DCreate9Orig = nullptr;

IDirect3D9* WINAPI Direct3DCreate9Hook(UINT SDKVersion)
{
	IDirect3D9* d3d9 = direct3DCreate9Orig(SDKVersion);
	if (d3d9 != nullptr)
		d3d9 = new th::MyDirect3D9(d3d9);
	return d3d9;
}

using DirectInput8Create_t = decltype(&DirectInput8Create);
DirectInput8Create_t directInput8CreateOrig = nullptr;

HRESULT WINAPI DirectInput8CreateHook(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
	HRESULT hr = directInput8CreateOrig(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	if (SUCCEEDED(hr))
	{
		if (riidltf == IID_IDirectInput8A)
			*ppvOut = new th::MyDirectInput8A(reinterpret_cast<IDirectInput8A*>(*ppvOut));
	}
	return hr;
}

void Hook()
{
	try
	{
		std::string logPath = base::win::Module(g_module).getDir() + "/Th10Hook_%N.log";
		logger.addFileLog(logPath);

		base::win::Module d3d9Dll = base::win::Module::Get("d3d9.dll");
		direct3DCreate9Orig =
			d3d9Dll.getProcAddress<Direct3DCreate9_t>("Direct3DCreate9");

		base::win::Module dinput8Dll = base::win::Module::Get("dinput8.dll");
		directInput8CreateOrig =
			dinput8Dll.getProcAddress<DirectInput8Create_t>("DirectInput8Create");

		th::MyDetours::TransactionBegin();
		th::MyDetours::Attach(reinterpret_cast<PVOID*>(&direct3DCreate9Orig), &Direct3DCreate9Hook);
		th::MyDetours::Attach(reinterpret_cast<PVOID*>(&directInput8CreateOrig), &DirectInput8CreateHook);
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
	{
		g_module = module;
		//th::DllLoader& dllLoader = th::DllLoader::GetInstance();
		//dllLoader.start();
		Hook();
		break;
	}
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

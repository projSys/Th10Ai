#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <atlbase.h>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <Base/Singleton.h>

#include "GraphHook/HookFunc.h"
#include "GraphHook/Input.h"

namespace gh
{
	namespace bip = boost::interprocess;

	struct DI8Remover
	{
		DI8Remover() { bip::shared_memory_object::remove("DI8SharedMemory"); }
		~DI8Remover() { bip::shared_memory_object::remove("DI8SharedMemory"); }
	};

	struct DI8SharedData
	{
		bip::interprocess_mutex keyMutex;
		KeyState keyState[256];
	};

	class DI8Hook :
		private Singleton<DI8Hook>
	{
	public:
		DI8Hook();
		~DI8Hook();

	private:
		// IDirectInput8
		typedef HRESULT(WINAPI *DirectInput8Create_t)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
		// IDirectInputDevice8
		typedef HRESULT(STDMETHODCALLTYPE *GetDeviceState_t)(IDirectInputDevice8*, DWORD, LPVOID);

		static HRESULT STDMETHODCALLTYPE GetDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID lpvData);

		HRESULT getDeviceStateHook(IDirectInputDevice8* device, DWORD cbData, LPVOID lpvData);

		DI8Remover m_remover;
		bip::managed_shared_memory m_memory;
		DI8SharedData* m_data;
		KeyState m_keyState[256];

		HookFunc<GetDeviceState_t> m_getDeviceStateFunc;
	};
}

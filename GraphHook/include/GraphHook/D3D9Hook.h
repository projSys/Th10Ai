#pragma once

#include <d3d9.h>
#include <atlbase.h>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <Base/Singleton.h>

#include "GraphHook/HookFunc.h"

namespace gh
{
	namespace bip = boost::interprocess;

	struct D3D9Remover
	{
		D3D9Remover() { bip::shared_memory_object::remove("D3D9SharedMemory"); }
		~D3D9Remover() { bip::shared_memory_object::remove("D3D9SharedMemory"); }
	};

	struct D3D9SharedData
	{
		bip::interprocess_mutex presentMutex;
		bip::interprocess_condition presentCond;
	};

	class D3D9Hook :
		private Singleton<D3D9Hook>
	{
	public:
		D3D9Hook();
		~D3D9Hook();

	private:
		// IDirect3D9
		typedef IDirect3D9* (WINAPI *Direct3DCreate9_t)(UINT);
		// IDirect3DDevice9
		typedef HRESULT(STDMETHODCALLTYPE *Reset_t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		typedef HRESULT(STDMETHODCALLTYPE *Present_t)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);

		static HRESULT STDMETHODCALLTYPE ResetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentationParameters);
		static HRESULT STDMETHODCALLTYPE PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		HRESULT resetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentationParameters);
		HRESULT presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		D3D9Remover m_remover;
		bip::managed_shared_memory m_memory;
		D3D9SharedData* m_data;

		HookFunc<Reset_t> m_resetFunc;
		HookFunc<Present_t> m_presentFunc;
	};
}

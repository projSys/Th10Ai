#pragma once

#include <d3d9.h>
#include <atlbase.h>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <Base/Singleton.h>

#include "libTH10AI/ApiHook/HookFunc.h"

namespace th
{
	namespace bip = boost::interprocess;

	struct D3D9FSRemover
	{
		D3D9FSRemover() { bip::shared_memory_object::remove("D3D9FSSharedMemory"); }
		~D3D9FSRemover() { bip::shared_memory_object::remove("D3D9FSSharedMemory"); }
	};

	struct D3D9FSSharedData
	{
		bip::interprocess_mutex presentBeginMutex;
		bip::interprocess_condition presentBeginCond;
		bool presentBeginReady;
		bip::interprocess_mutex presentEndMutex;
		bip::interprocess_condition presentEndCond;
		bool presentEndReady;
		std::chrono::steady_clock::time_point m_t0;
	};

	class D3D9Hook :
		private Singleton<D3D9Hook>
	{
	public:
		D3D9Hook();
		~D3D9Hook();

		bool waitForPresent();

	private:
		// IDirect3D9
		typedef IDirect3D9* (WINAPI *Direct3DCreate9_t)(UINT);
		// IDirect3DDevice9
		typedef HRESULT(STDMETHODCALLTYPE *Clear_t)(IDirect3DDevice9*, DWORD, CONST D3DRECT*, DWORD, D3DCOLOR, float, DWORD);
		typedef HRESULT(STDMETHODCALLTYPE *BeginScene_t)(IDirect3DDevice9*);
		typedef HRESULT(STDMETHODCALLTYPE *EndScene_t)(IDirect3DDevice9*);
		typedef HRESULT(STDMETHODCALLTYPE *Present_t)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);

		static HRESULT STDMETHODCALLTYPE ClearHook(IDirect3DDevice9* device, DWORD count, CONST D3DRECT* rects, DWORD flags,
			D3DCOLOR color, float z, DWORD stencil);
		static HRESULT STDMETHODCALLTYPE BeginSceneHook(IDirect3DDevice9* device);
		static HRESULT STDMETHODCALLTYPE EndSceneHook(IDirect3DDevice9* device);
		static HRESULT STDMETHODCALLTYPE PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		HRESULT clearHook(IDirect3DDevice9* device, DWORD count, CONST D3DRECT* rects, DWORD flags,
			D3DCOLOR color, float z, DWORD stencil);
		HRESULT beginSceneHook(IDirect3DDevice9* device);
		HRESULT endSceneHook(IDirect3DDevice9* device);
		HRESULT presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		D3D9FSRemover m_remover;
		bip::managed_shared_memory m_memory;
		D3D9FSSharedData* m_data;

		std::mutex m_presentBeginMutex;
		std::condition_variable m_presentBeginCond;
		bool m_presentBeginReady;

		HookFunc<Clear_t> m_clear;
		HookFunc<BeginScene_t> m_beginScene;
		HookFunc<EndScene_t> m_endScene;
		HookFunc<Present_t> m_present;

		std::chrono::steady_clock::time_point m_t0;
	};
}

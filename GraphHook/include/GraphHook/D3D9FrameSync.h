#pragma once

#include <chrono>
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

	class D3D9FrameSync :
		private Singleton<D3D9FrameSync>
	{
	public:
		D3D9FrameSync();
		~D3D9FrameSync();

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

		HookFunc<Clear_t> m_clearFunc;
		HookFunc<BeginScene_t> m_beginSceneFunc;
		HookFunc<EndScene_t> m_endSceneFunc;
		HookFunc<Present_t> m_presentFunc;

		std::chrono::steady_clock::time_point m_t0;
	};
}

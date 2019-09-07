#pragma once

#include <d3d9.h>
#include <atlbase.h>
#include <mutex>
#include <condition_variable>
#include <Base/Singleton.h>

#include "libTH10AI/ApiHook/HookFunc.h"

namespace th
{
	class D3D9Hook :
		private Singleton<D3D9Hook>
	{
	public:
		D3D9Hook();
		~D3D9Hook();

		void notifyPresentBegin();
		bool waitPresentBegin();
		void notifyPresentEnd();
		bool waitPresentEnd();

		time_t getPresentTimespan() const;

	private:
		// IDirect3D9
		typedef IDirect3D9* (WINAPI *Direct3DCreate9_t)(UINT);
		// IDirect3DDevice9
		typedef HRESULT(STDMETHODCALLTYPE *BeginScene_t)(IDirect3DDevice9*);
		typedef HRESULT(STDMETHODCALLTYPE *EndScene_t)(IDirect3DDevice9*);
		typedef HRESULT(STDMETHODCALLTYPE *Clear_t)(IDirect3DDevice9*, DWORD, CONST D3DRECT*, DWORD, D3DCOLOR, float, DWORD);
		typedef HRESULT(STDMETHODCALLTYPE *Present_t)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);

		static HRESULT STDMETHODCALLTYPE BeginSceneHook(IDirect3DDevice9* device);
		static HRESULT STDMETHODCALLTYPE EndSceneHook(IDirect3DDevice9* device);
		static HRESULT STDMETHODCALLTYPE ClearHook(IDirect3DDevice9* device, DWORD count, CONST D3DRECT* rects, DWORD flags,
			D3DCOLOR color, float z, DWORD stencil);
		static HRESULT STDMETHODCALLTYPE PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		HRESULT beginSceneHook(IDirect3DDevice9* device);
		HRESULT endSceneHook(IDirect3DDevice9* device);
		HRESULT clearHook(IDirect3DDevice9* device, DWORD count, CONST D3DRECT* rects, DWORD flags,
			D3DCOLOR color, float z, DWORD stencil);
		HRESULT presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		std::mutex m_presentBeginMutex;
		std::condition_variable m_presentBeginCond;
		bool m_presentBeginReady;
		std::mutex m_presentEndMutex;
		std::condition_variable m_presentEndCond;
		bool m_presentEndReady;

		std::chrono::steady_clock::time_point m_beginSceneTime;
		std::chrono::steady_clock::time_point m_endSceneTime;
		std::chrono::steady_clock::time_point m_clearTime;
		std::chrono::steady_clock::time_point m_presentBeginTime;
		std::chrono::steady_clock::time_point m_presentEndTime;
		time_t m_presentTimespan;

		HookFunc<BeginScene_t> m_beginScene;
		HookFunc<EndScene_t> m_endScene;
		HookFunc<Clear_t> m_clear;
		HookFunc<Present_t> m_present;
	};
}

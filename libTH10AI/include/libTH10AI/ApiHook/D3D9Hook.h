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

	private:
		// IDirect3D9
		typedef IDirect3D9* (WINAPI *Direct3DCreate9_t)(UINT);
		// IDirect3DDevice9
		typedef HRESULT(STDMETHODCALLTYPE *Present_t)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);

		static HRESULT STDMETHODCALLTYPE PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		HRESULT presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		std::mutex m_presentBeginMutex;
		std::condition_variable m_presentBeginCond;
		bool m_presentBeginReady;
		std::mutex m_presentEndMutex;
		std::condition_variable m_presentEndCond;
		bool m_presentEndReady;

		HookFunc<Present_t> m_present;
	};

	extern std::chrono::steady_clock::time_point g_presentBeginTime;
	extern std::chrono::steady_clock::time_point g_presentEndTime;
}

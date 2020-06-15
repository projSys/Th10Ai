#pragma once

#include <d3d9.h>
#include <atlbase.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <Base/Singleton.h>

namespace th
{
	class D3D9Hook :
		private Singleton<D3D9Hook>
	{
	public:
		D3D9Hook();
		~D3D9Hook();

		void enable(bool enabled);

		void notifyPresent();
		bool waitPresent();

	private:
		// IDirect3D9
		using Direct3DCreate9_t = decltype(Direct3DCreate9);
		// IDirect3DDevice9
		using Present_t = HRESULT STDMETHODCALLTYPE(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);

		static HRESULT STDMETHODCALLTYPE PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		HRESULT presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		std::atomic_bool m_enabled;
		Present_t* m_present;

		std::mutex m_presentMutex;
		std::condition_variable m_presentCond;
		bool m_presentReadied;
	};

	extern std::chrono::steady_clock::time_point g_presentBeginTime;
	extern std::chrono::steady_clock::time_point g_presentEndTime;
}

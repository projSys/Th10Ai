#pragma once

#include <d3d9.h>
#include <atlbase.h>
#include <atomic>
#include <Base/Singleton.h>

namespace th
{
	class D3D9Listener
	{
	public:
		virtual ~D3D9Listener() = default;

		virtual void onPresentBegin(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion) {}
		virtual void onPresentEnd(HRESULT& hr, IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion) {}
	};

	class D3D9Hook :
		private Singleton<D3D9Hook>
	{
	public:
		D3D9Hook(D3D9Listener* listener);
		~D3D9Hook();

		void enable(bool enabled);

	private:
		// IDirect3D9
		typedef IDirect3D9* (WINAPI *Direct3DCreate9_t)(UINT);
		// IDirect3DDevice9
		typedef HRESULT(STDMETHODCALLTYPE *Present_t)(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);

		static HRESULT STDMETHODCALLTYPE PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		HRESULT presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		D3D9Listener* m_listener;
		std::atomic_bool m_enabled;
		Present_t m_present;
	};
}

#pragma once

#include "Th10Hook/Common.h"

#include <d3d9.h>
#include <atlbase.h>
#include <Base/Singleton.h>

namespace th
{
	class D3D9Listener
	{
	public:
		virtual ~D3D9Listener() = default;
		virtual void onPresent(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion) = 0;
	};

	class D3D9Hook :
		private Singleton<D3D9Hook>
	{
	public:
		D3D9Hook(D3D9Listener* listener);
		~D3D9Hook();

	private:
		// IDirect3D9
		using Direct3DCreate9_t = decltype(Direct3DCreate9);
		// IDirect3DDevice9
		using Present_t = HRESULT STDMETHODCALLTYPE(IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);

		static HRESULT STDMETHODCALLTYPE PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		HRESULT presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion);

		D3D9Listener* m_listener;
		Present_t* m_present;
	};
}

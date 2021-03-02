#pragma once

#include "Th10Hook/Common.h"

#include <d3d9.h>
#include <atlbase.h>
#include <Base/TypeTraits.h>
#include <Base/Singleton.h>

namespace th
{
	class D3D9Listener
	{
	public:
		virtual ~D3D9Listener() = default;
		virtual void onPresent(IDirect3DDevice9* device, const RECT* sourceRect, const RECT* destRect,
			HWND destWindowOverride, const RGNDATA* dirtyRegion) = 0;
	};

	class D3D9Hook :
		private Singleton<D3D9Hook>
	{
	public:
		D3D9Hook(D3D9Listener* listener);
		~D3D9Hook();

	private:
		// IDirect3D9
		using Direct3DCreate9_t = decltype(&Direct3DCreate9);
		// IDirect3DDevice9
		using Present_t = FuncTraits<decltype(&IDirect3DDevice9::Present)>::Raw_t;

		static HRESULT __stdcall PresentHook(IDirect3DDevice9* device, const RECT* sourceRect, const RECT* destRect,
			HWND destWindowOverride, const RGNDATA* dirtyRegion);

		HRESULT presentHook(IDirect3DDevice9* device, const RECT* sourceRect, const RECT* destRect,
			HWND destWindowOverride, const RGNDATA* dirtyRegion);

		D3D9Listener* m_listener;
		Present_t m_presentOrig;
	};
}

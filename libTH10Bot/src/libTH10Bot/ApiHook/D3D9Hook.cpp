#include "libTH10Bot/Common.h"
#include "libTH10Bot/ApiHook/D3D9Hook.h"

#include <Base/ScopeGuard.h>

namespace th
{
	D3D9Hook::D3D9Hook(D3D9Listener* listener) :
		Singleton(this),
		m_listener(listener),
		m_enabled(false)
	{
		if (listener == nullptr)
			THROW_BASE_EXCEPTION(Exception() << err_str("D3D9Listener is null."));

		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = &DefWindowProc;
		wcex.hInstance = GetModuleHandle(nullptr);
		wcex.lpszClassName = _T("D3D9WndClass");
		if (RegisterClassEx(&wcex) == 0)
			THROW_WINDOWS_ERROR(GetLastError());
		ON_SCOPE_EXIT([&]()
		{
			UnregisterClass(wcex.lpszClassName, wcex.hInstance);
		});

		HWND window = CreateWindowEx(0, wcex.lpszClassName, _T("D3D9Wnd"), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, wcex.hInstance, nullptr);
		if (window == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		ON_SCOPE_EXIT([&]()
		{
			DestroyWindow(window);
		});

		HMODULE d3d9Dll = GetModuleHandle(_T("d3d9.dll"));
		if (d3d9Dll == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		Direct3DCreate9_t direct3DCreate9 = reinterpret_cast<Direct3DCreate9_t>(
			GetProcAddress(d3d9Dll, "Direct3DCreate9"));
		if (direct3DCreate9 == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());

		CComPtr<IDirect3D9> d3d9;
		d3d9.p = direct3DCreate9(D3D_SDK_VERSION);
		if (d3d9 == nullptr)
			THROW_BASE_EXCEPTION(Exception() << err_str("Direct3DCreate9() failed."));

		D3DDISPLAYMODE d3ddm = {};
		HRESULT hr = d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		D3DPRESENT_PARAMETERS d3dpp = {};
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = d3ddm.Format;

		CComPtr<IDirect3DDevice9> device;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
			&d3dpp, &device);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		uint_t* vTable = (uint_t*)(*((uint_t*)device.p));
		m_present = HookFunc<Present_t>(reinterpret_cast<LPVOID>(vTable[17]), &D3D9Hook::PresentHook);
	}

	void D3D9Hook::enable(bool enabled)
	{
		m_enabled = enabled;
	}

	HRESULT D3D9Hook::PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		D3D9Hook& d3d9Hook = D3D9Hook::GetInstance();
		return d3d9Hook.presentHook(device, sourceRect, destRect, destWindowOverride, dirtyRegion);
	}

	HRESULT D3D9Hook::presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		if (!m_enabled)
			return m_present(device, sourceRect, destRect, destWindowOverride, dirtyRegion);

		m_listener->onPresentBegin(device, sourceRect, destRect, destWindowOverride, dirtyRegion);
		HRESULT hr = m_present(device, sourceRect, destRect, destWindowOverride, dirtyRegion);
		m_listener->onPresentEnd(hr, device, sourceRect, destRect, destWindowOverride, dirtyRegion);
		return hr;
	}
}

#include "Th10Hook/D3D9Hook.h"

#include <Base/ScopeGuard.h>
#include <Base/Windows/DxResult.h>

#include "Th10Hook/MyDetours.h"

namespace th
{
	D3D9Hook::D3D9Hook(D3D9Listener* listener) :
		Singleton(this),
		m_listener(listener),
		m_presentOrig(nullptr)
	{
		WNDCLASSEXW wc = {};
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = &DefWindowProcW;
		wc.hInstance = GetModuleHandleW(nullptr);
		wc.lpszClassName = L"D3D9HookClass";
		if (RegisterClassExW(&wc) == 0)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				UnregisterClassW(wc.lpszClassName, wc.hInstance);
			});

		HWND window = CreateWindowExW(0, wc.lpszClassName, L"D3D9HookWindow",
			WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, nullptr, nullptr, wc.hInstance, nullptr);
		if (window == nullptr)
			BASE_THROW(WindowsError());
		ON_SCOPE_EXIT([&]()
			{
				DestroyWindow(window);
			});

		HMODULE d3d9Dll = GetModuleHandleW(L"d3d9.dll");
		if (d3d9Dll == nullptr)
			BASE_THROW(WindowsError());
		Direct3DCreate9_t direct3DCreate9 = reinterpret_cast<Direct3DCreate9_t>(
			GetProcAddress(d3d9Dll, "Direct3DCreate9"));
		if (direct3DCreate9 == nullptr)
			BASE_THROW(WindowsError());

		CComPtr<IDirect3D9> d3d9;
		d3d9.p = direct3DCreate9(D3D_SDK_VERSION);
		if (d3d9 == nullptr)
			BASE_THROW(Exception(u8"Direct3DCreate9()调用失败。"));

		D3DDISPLAYMODE d3ddm = {};
		HRESULT hr = d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
		if (FAILED(hr))
			BASE_THROW(DxResult(hr, u8"GetAdapterDisplayMode()调用失败。"));

		D3DPRESENT_PARAMETERS d3dpp = {};
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = d3ddm.Format;

		CComPtr<IDirect3DDevice9> device;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device);
		if (FAILED(hr))
			BASE_THROW(DxResult(hr, u8"CreateDevice()调用失败。"));

		uint_t* vTable = reinterpret_cast<uint_t*>(*reinterpret_cast<uint_t*>(device.p));
		m_presentOrig = reinterpret_cast<Present_t>(vTable[17]);

		MyDetours::TransactionBegin();
		MyDetours::Attach(reinterpret_cast<PVOID*>(&m_presentOrig), &D3D9Hook::PresentHook);
		MyDetours::TransactionCommit();
	}

	D3D9Hook::~D3D9Hook()
	{
		try
		{
			MyDetours::TransactionBegin();
			MyDetours::Detach(reinterpret_cast<PVOID*>(&m_presentOrig), &D3D9Hook::PresentHook);
			MyDetours::TransactionCommit();
		}
		catch (...)
		{
			BASE_LOG_FATAL(PrintException());
		}
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
		try
		{
			m_listener->onPresent(device, sourceRect, destRect, destWindowOverride, dirtyRegion);
		}
		catch (...)
		{
			BASE_LOG_ERROR(PrintException());
		}

		return m_presentOrig(device, sourceRect, destRect, destWindowOverride, dirtyRegion);
	}
}

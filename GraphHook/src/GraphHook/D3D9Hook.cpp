#include "GraphHook/Common.h"
#include "GraphHook/D3D9Hook.h"

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <Base/ScopeGuard.h>

namespace gh
{
	D3D9Hook::D3D9Hook() :
		Singleton(this)
	{
		m_memory = bip::managed_shared_memory(bip::create_only, "D3D9SharedMemory", 65536);
		m_data = m_memory.construct<D3D9SharedData>("D3D9SharedData")();

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

		D3DDISPLAYMODE mode = {};
		HRESULT hr = d3d9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		D3DPRESENT_PARAMETERS pp = {};
		pp.Windowed = TRUE;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.BackBufferFormat = mode.Format;

		CComPtr<IDirect3DDevice9> device;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
			&pp, &device);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		uint_t* vTable = (uint_t*)(*((uint_t*)device.p));
		m_resetFunc = HookFunc<Reset_t>(reinterpret_cast<LPVOID>(vTable[16]), &D3D9Hook::ResetHook);
		m_presentFunc = HookFunc<Present_t>(reinterpret_cast<LPVOID>(vTable[17]), &D3D9Hook::PresentHook);
	}

	D3D9Hook::~D3D9Hook()
	{
		m_memory.destroy<D3D9SharedData>("D3D9SharedData");
	}

	HRESULT D3D9Hook::ResetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentationParameters)
	{
		D3D9Hook& d3d9Hook = D3D9Hook::GetInstance();
		return d3d9Hook.resetHook(device, presentationParameters);
	}

	HRESULT D3D9Hook::PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		D3D9Hook& d3d9Hook = D3D9Hook::GetInstance();
		return d3d9Hook.presentHook(device, sourceRect, destRect, destWindowOverride, dirtyRegion);
	}

	HRESULT D3D9Hook::resetHook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentationParameters)
	{
		return m_resetFunc(device, presentationParameters);
	}

	HRESULT D3D9Hook::presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		try
		{
			//CComPtr<IDirect3DSurface9> renderTarget;
			//HRESULT hr = device->GetRenderTarget(0, &renderTarget);
			//if (FAILED(hr))
			//	THROW_DIRECTX_HRESULT(hr);

			//if (m_destSurface == nullptr)
			//{
			//	D3DSURFACE_DESC desc = {};
			//	hr = renderTarget->GetDesc(&desc);
			//	if (FAILED(hr))
			//		THROW_DIRECTX_HRESULT(hr);

			//	hr = device->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format,
			//		D3DPOOL_SYSTEMMEM, &m_destSurface, nullptr);
			//	if (FAILED(hr))
			//		THROW_DIRECTX_HRESULT(hr);

			//	D3DLOCKED_RECT lockedRect = {};
			//	hr = m_destSurface->LockRect(&lockedRect, nullptr, D3DLOCK_READONLY);
			//	if (FAILED(hr))
			//		THROW_DIRECTX_HRESULT(hr);
			//	ON_SCOPE_EXIT([&]()
			//	{
			//		m_destSurface->UnlockRect();
			//	});

			//	m_info.format = desc.Format;
			//	m_info.width = desc.Width;
			//	m_info.height = desc.Height;
			//	m_info.pitch = lockedRect.Pitch;
			//	memcpy(m_infoRegion.get_address(), &m_info, sizeof(m_info));

			//	m_dataMemory = bip::windows_shared_memory(bip::create_only, "D3D9HookData",
			//		bip::read_write, m_info.pitch * m_info.height);
			//	m_dataRegion = bip::mapped_region(m_dataMemory, bip::read_write);
			//}

			//hr = device->GetRenderTargetData(renderTarget, m_destSurface);
			//if (FAILED(hr))
			//	THROW_DIRECTX_HRESULT(hr);

			//D3DLOCKED_RECT lockedRect = {};
			//hr = m_destSurface->LockRect(&lockedRect, nullptr, D3DLOCK_READONLY);
			//if (FAILED(hr))
			//	THROW_DIRECTX_HRESULT(hr);
			//ON_SCOPE_EXIT([&]()
			//{
			//	m_destSurface->UnlockRect();
			//});

			//memcpy(m_dataRegion.get_address(), lockedRect.pBits, m_info.pitch * m_info.height);

			bip::scoped_lock<bip::interprocess_mutex> lock(m_data->presentMutex);
			m_data->presentCond.notify_one();
		}
		catch (...)
		{
			//std::string what = boost::current_exception_diagnostic_information();
			//BOOST_LOG_TRIVIAL(error) << what;
		}

		return m_presentFunc(device, sourceRect, destRect, destWindowOverride, dirtyRegion);
	}
}

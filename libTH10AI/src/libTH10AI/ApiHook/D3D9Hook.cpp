#include "libTH10AI/Common.h"
#include "libTH10AI/ApiHook/D3D9Hook.h"

#include <Base/ScopeGuard.h>

namespace th
{
	D3D9Hook::D3D9Hook() :
		Singleton(this),
		m_presentBeginReady(false),
		m_presentEndReady(false),
		m_presentTimespan(0)
	{
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = &DefWindowProc;
		wcex.hInstance = GetModuleHandle(nullptr);
		wcex.lpszClassName = _T("D3D9FSWndClass");
		if (RegisterClassEx(&wcex) == 0)
			THROW_WINDOWS_ERROR(GetLastError());
		ON_SCOPE_EXIT([&]()
		{
			UnregisterClass(wcex.lpszClassName, wcex.hInstance);
		});

		HWND window = CreateWindowEx(0, wcex.lpszClassName, _T("D3D9FSWnd"), WS_OVERLAPPEDWINDOW,
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
		//m_beginScene = HookFunc<BeginScene_t>(reinterpret_cast<LPVOID>(vTable[41]), &D3D9Hook::BeginSceneHook);
		//m_endScene = HookFunc<EndScene_t>(reinterpret_cast<LPVOID>(vTable[42]), &D3D9Hook::EndSceneHook);
		//m_clear = HookFunc<Clear_t>(reinterpret_cast<LPVOID>(vTable[43]), &D3D9Hook::ClearHook);
		m_present = HookFunc<Present_t>(reinterpret_cast<LPVOID>(vTable[17]), &D3D9Hook::PresentHook);
	}

	D3D9Hook::~D3D9Hook()
	{
	}

	HRESULT D3D9Hook::BeginSceneHook(IDirect3DDevice9* device)
	{
		D3D9Hook& d3d9Hook = D3D9Hook::GetInstance();
		return d3d9Hook.beginSceneHook(device);
	}

	HRESULT D3D9Hook::EndSceneHook(IDirect3DDevice9* device)
	{
		D3D9Hook& d3d9Hook = D3D9Hook::GetInstance();
		return d3d9Hook.endSceneHook(device);
	}

	HRESULT D3D9Hook::ClearHook(IDirect3DDevice9* device, DWORD count, CONST D3DRECT* rects, DWORD flags,
		D3DCOLOR color, float z, DWORD stencil)
	{
		D3D9Hook& d3d9Hook = D3D9Hook::GetInstance();
		return d3d9Hook.clearHook(device, count, rects, flags, color, z, stencil);
	}

	HRESULT D3D9Hook::PresentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		D3D9Hook& d3d9Hook = D3D9Hook::GetInstance();
		return d3d9Hook.presentHook(device, sourceRect, destRect, destWindowOverride, dirtyRegion);
	}

	HRESULT D3D9Hook::beginSceneHook(IDirect3DDevice9* device)
	{
		m_beginSceneTime = std::chrono::steady_clock::now();
		return m_beginScene(device);
	}

	HRESULT D3D9Hook::endSceneHook(IDirect3DDevice9* device)
	{
		m_endSceneTime = std::chrono::steady_clock::now();
		return m_endScene(device);
	}

	HRESULT D3D9Hook::clearHook(IDirect3DDevice9* device, DWORD count, CONST D3DRECT* rects, DWORD flags,
		D3DCOLOR color, float z, DWORD stencil)
	{
		m_clearTime = std::chrono::steady_clock::now();
		return m_clear(device, count, rects, flags, color, z, stencil);
	}

	// 帧同步失败
	// 垂直同步的等待时间会在0-15之间递增或递减，或者长时间停留在0-1。
	// 也就是说没时间读取数据和计算。
	HRESULT D3D9Hook::presentHook(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		m_presentBeginTime = std::chrono::steady_clock::now();
		notifyPresentBegin();
		HRESULT hr = m_present(device, sourceRect, destRect, destWindowOverride, dirtyRegion);
		m_presentEndTime = std::chrono::steady_clock::now();
		std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
			m_presentEndTime - m_presentBeginTime);
		m_presentTimespan = interval.count();
		std::cout << m_presentTimespan << std::endl;
		//BOOST_LOG_TRIVIAL(error) << m_presentTimespan;
		//notifyPresentEnd();
		return hr;
	}

	void D3D9Hook::notifyPresentBegin()
	{
		std::unique_lock<std::mutex> lock(m_presentBeginMutex);
		m_presentBeginReady = true;
		m_presentBeginCond.notify_one();
	}

	bool D3D9Hook::waitPresentBegin()
	{
		bool waited = false;
		std::unique_lock<std::mutex> lock(m_presentBeginMutex);
		while (!m_presentBeginReady)
		{
			m_presentBeginCond.wait(lock);
			waited = true;
		}
		m_presentBeginReady = false;
		return waited;
	}

	void D3D9Hook::notifyPresentEnd()
	{
		std::unique_lock<std::mutex> lock(m_presentEndMutex);
		m_presentEndReady = true;
		m_presentEndCond.notify_one();
	}

	bool D3D9Hook::waitPresentEnd()
	{
		bool waited = false;
		std::unique_lock<std::mutex> lock(m_presentEndMutex);
		while (!m_presentEndReady)
		{
			m_presentEndCond.wait(lock);
			waited = true;
		}
		m_presentEndReady = false;
		return waited;
	}

	time_t D3D9Hook::getPresentTimespan() const
	{
		return m_presentTimespan;
	}
}

#include "Th10Hook/DirectX/MyDirect3D9.h"

#include "Th10Hook/DirectX/MyDirect3DDevice9.h"

namespace th
{
	MyDirect3D9::MyDirect3D9(IDirect3D9* d3d9) :
		m_d3d9(d3d9)
	{
	}

	HRESULT MyDirect3D9::QueryInterface(REFIID riid, void** ppvObj)
	{
		HRESULT hr = m_d3d9->QueryInterface(riid, ppvObj);
		if (SUCCEEDED(hr))
			*ppvObj = this;
		return hr;
	}

	ULONG MyDirect3D9::AddRef()
	{
		return m_d3d9->AddRef();
	}

	ULONG MyDirect3D9::Release()
	{
		ULONG refCount = m_d3d9->Release();
		if (refCount == 0)
			delete this;
		return refCount;
	}

	HRESULT MyDirect3D9::RegisterSoftwareDevice(void* pInitializeFunction)
	{
		return m_d3d9->RegisterSoftwareDevice(pInitializeFunction);
	}

	UINT MyDirect3D9::GetAdapterCount()
	{
		return m_d3d9->GetAdapterCount();
	}

	HRESULT MyDirect3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
	{
		return m_d3d9->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
	}

	UINT MyDirect3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
	{
		return m_d3d9->GetAdapterModeCount(Adapter, Format);
	}

	HRESULT MyDirect3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
	{
		return m_d3d9->EnumAdapterModes(Adapter, Format, Mode, pMode);
	}

	HRESULT MyDirect3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
	{
		return m_d3d9->GetAdapterDisplayMode(Adapter, pMode);
	}

	HRESULT MyDirect3D9::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
	{
		return m_d3d9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
	}

	HRESULT MyDirect3D9::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
	{
		return m_d3d9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
	}

	HRESULT MyDirect3D9::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
	{
		return m_d3d9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
	}

	HRESULT MyDirect3D9::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
	{
		return m_d3d9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
	}

	HRESULT MyDirect3D9::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
	{
		return m_d3d9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
	}

	HRESULT MyDirect3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
	{
		return m_d3d9->GetDeviceCaps(Adapter, DeviceType, pCaps);
	}

	HMONITOR MyDirect3D9::GetAdapterMonitor(UINT Adapter)
	{
		return m_d3d9->GetAdapterMonitor(Adapter);
	}

	HRESULT MyDirect3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
	{
		HRESULT hr = m_d3d9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
		if (SUCCEEDED(hr))
			*ppReturnedDeviceInterface = new MyDirect3DDevice9(*ppReturnedDeviceInterface, this);
		return hr;
	}
}

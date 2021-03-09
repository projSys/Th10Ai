#include "Th10Hook/DirectX/MyDirectInput8A.h"

#include "Th10Hook/DirectX/MyDirectInputDevice8A.h"

namespace th
{
	MyDirectInput8A::MyDirectInput8A(IDirectInput8A* di8) :
		m_di8(di8)
	{
	}

	HRESULT MyDirectInput8A::QueryInterface(REFIID riid, LPVOID* ppvObj)
	{
		HRESULT hr = m_di8->QueryInterface(riid, ppvObj);
		if (SUCCEEDED(hr))
			*ppvObj = this;
		return hr;
	}

	ULONG MyDirectInput8A::AddRef()
	{
		return m_di8->AddRef();
	}

	ULONG MyDirectInput8A::Release()
	{
		ULONG refCount = m_di8->Release();
		if (refCount == 0)
			delete this;
		return refCount;
	}

	HRESULT MyDirectInput8A::CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8A* lplpDirectInputDevice, LPUNKNOWN pUnkOuter)
	{
		HRESULT hr = m_di8->CreateDevice(rguid, lplpDirectInputDevice, pUnkOuter);
		if (SUCCEEDED(hr))
			*lplpDirectInputDevice = new MyDirectInputDevice8A(*lplpDirectInputDevice, this);
		return hr;
	}

	HRESULT MyDirectInput8A::EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags)
	{
		return m_di8->EnumDevices(dwDevType, lpCallback, pvRef, dwFlags);
	}

	HRESULT MyDirectInput8A::GetDeviceStatus(REFGUID rguidInstance)
	{
		return m_di8->GetDeviceStatus(rguidInstance);
	}

	HRESULT MyDirectInput8A::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
	{
		return m_di8->RunControlPanel(hwndOwner, dwFlags);
	}

	HRESULT MyDirectInput8A::Initialize(HINSTANCE hinst, DWORD dwVersion)
	{
		return m_di8->Initialize(hinst, dwVersion);
	}

	HRESULT MyDirectInput8A::FindDevice(REFGUID rguidClass, LPCSTR ptszName, LPGUID pguidInstance)
	{
		return m_di8->FindDevice(rguidClass, ptszName, pguidInstance);
	}

	HRESULT MyDirectInput8A::EnumDevicesBySemantics(LPCSTR ptszUserName, LPDIACTIONFORMATA lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBA lpCallback, LPVOID pvRef, DWORD dwFlags)
	{
		return m_di8->EnumDevicesBySemantics(ptszUserName, lpdiActionFormat, lpCallback, pvRef, dwFlags);
	}

	HRESULT MyDirectInput8A::ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSA lpdiCDParams, DWORD dwFlags, LPVOID pvRefData)
	{
		return m_di8->ConfigureDevices(lpdiCallback, lpdiCDParams, dwFlags, pvRefData);
	}
}

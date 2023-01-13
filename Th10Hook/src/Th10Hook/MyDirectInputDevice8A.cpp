#include "Th10Hook/MyDirectInputDevice8A.h"

#include <Base/Catcher.h>
#include <Base/Time.h>

#include "Th10Hook/MyDirectInput8A.h"
#include "Th10Hook/Th10Hook.h"

namespace th
{
	MyDirectInputDevice8A::MyDirectInputDevice8A(IDirectInputDevice8A* device, MyDirectInput8A* di8) :
		m_device(device),
		m_di8(di8)
	{
	}

	HRESULT MyDirectInputDevice8A::QueryInterface(REFIID riid, LPVOID* ppvObj)
	{
		HRESULT hr = m_device->QueryInterface(riid, ppvObj);
		if (SUCCEEDED(hr))
			*ppvObj = this;
		return hr;
	}

	ULONG MyDirectInputDevice8A::AddRef()
	{
		return m_device->AddRef();
	}

	ULONG MyDirectInputDevice8A::Release()
	{
		ULONG refCount = m_device->Release();
		if (refCount == 0)
			delete this;
		return refCount;
	}

	HRESULT MyDirectInputDevice8A::GetCapabilities(LPDIDEVCAPS lpDIDevCaps)
	{
		return m_device->GetCapabilities(lpDIDevCaps);
	}

	HRESULT MyDirectInputDevice8A::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwFlags)
	{
		return m_device->EnumObjects(lpCallback, pvRef, dwFlags);
	}

	HRESULT MyDirectInputDevice8A::GetProperty(REFGUID rguidProp, LPDIPROPHEADER pdiph)
	{
		return m_device->GetProperty(rguidProp, pdiph);
	}

	HRESULT MyDirectInputDevice8A::SetProperty(REFGUID rguidProp, LPCDIPROPHEADER pdiph)
	{
		return m_device->SetProperty(rguidProp, pdiph);
	}

	HRESULT MyDirectInputDevice8A::Acquire()
	{
		return m_device->Acquire();
	}

	HRESULT MyDirectInputDevice8A::Unacquire()
	{
		return m_device->Unacquire();
	}

	HRESULT MyDirectInputDevice8A::GetDeviceState(DWORD cbData, LPVOID lpvData)
	{
		HRESULT hr = m_device->GetDeviceState(cbData, lpvData);
		if (SUCCEEDED(hr))
		{
			//Time t1 = Clock::Now();

			try
			{
				if (g_th10Hook != nullptr)
					g_th10Hook->commitInputTo(cbData, lpvData);
			}
			catch (...)
			{
				BASE_LOG(error) << Catcher() << std::endl;
				ExitProcess(1);
			}

			//Time t2 = Clock::Now();
			//std::cout << t2 - t1 << std::endl;
		}
		return hr;
	}

	HRESULT MyDirectInputDevice8A::GetDeviceData(DWORD cbObjectData, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD dwFlags)
	{
		return m_device->GetDeviceData(cbObjectData, rgdod, pdwInOut, dwFlags);
	}

	HRESULT MyDirectInputDevice8A::SetDataFormat(LPCDIDATAFORMAT lpdf)
	{
		return m_device->SetDataFormat(lpdf);
	}

	HRESULT MyDirectInputDevice8A::SetEventNotification(HANDLE hEvent)
	{
		return m_device->SetEventNotification(hEvent);
	}

	HRESULT MyDirectInputDevice8A::SetCooperativeLevel(HWND hwnd, DWORD dwFlags)
	{
		return m_device->SetCooperativeLevel(hwnd, dwFlags);
	}

	HRESULT MyDirectInputDevice8A::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEA pdidoi, DWORD dwObj, DWORD dwHow)
	{
		return m_device->GetObjectInfo(pdidoi, dwObj, dwHow);
	}

	HRESULT MyDirectInputDevice8A::GetDeviceInfo(LPDIDEVICEINSTANCEA pdidi)
	{
		return m_device->GetDeviceInfo(pdidi);
	}

	HRESULT MyDirectInputDevice8A::RunControlPanel(HWND hwndOwner, DWORD dwFlags)
	{
		return m_device->RunControlPanel(hwndOwner, dwFlags);
	}

	HRESULT MyDirectInputDevice8A::Initialize(HINSTANCE hinst, DWORD dwVersion, REFGUID rguid)
	{
		return m_device->Initialize(hinst, dwVersion, rguid);
	}

	HRESULT MyDirectInputDevice8A::CreateEffect(REFGUID rguid, LPCDIEFFECT lpeff, LPDIRECTINPUTEFFECT* ppdeff, LPUNKNOWN punkOuter)
	{
		return m_device->CreateEffect(rguid, lpeff, ppdeff, punkOuter);
	}

	HRESULT MyDirectInputDevice8A::EnumEffects(LPDIENUMEFFECTSCALLBACKA lpCallback, LPVOID pvRef, DWORD dwEffType)
	{
		return m_device->EnumEffects(lpCallback, pvRef, dwEffType);
	}

	HRESULT MyDirectInputDevice8A::GetEffectInfo(LPDIEFFECTINFOA pdei, REFGUID rguid)
	{
		return m_device->GetEffectInfo(pdei, rguid);
	}

	HRESULT MyDirectInputDevice8A::GetForceFeedbackState(LPDWORD pdwOut)
	{
		return m_device->GetForceFeedbackState(pdwOut);
	}

	HRESULT MyDirectInputDevice8A::SendForceFeedbackCommand(DWORD dwFlags)
	{
		return m_device->SendForceFeedbackCommand(dwFlags);
	}

	HRESULT MyDirectInputDevice8A::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK lpCallback, LPVOID pvRef, DWORD fl)
	{
		return m_device->EnumCreatedEffectObjects(lpCallback, pvRef, fl);
	}

	HRESULT MyDirectInputDevice8A::Escape(LPDIEFFESCAPE pesc)
	{
		return m_device->Escape(pesc);
	}

	HRESULT MyDirectInputDevice8A::Poll()
	{
		return m_device->Poll();
	}

	HRESULT MyDirectInputDevice8A::SendDeviceData(DWORD cbObjectData, LPCDIDEVICEOBJECTDATA rgdod, LPDWORD pdwInOut, DWORD fl)
	{
		return m_device->SendDeviceData(cbObjectData, rgdod, pdwInOut, fl);
	}

	HRESULT MyDirectInputDevice8A::EnumEffectsInFile(LPCSTR lpszFileName, LPDIENUMEFFECTSINFILECALLBACK pec, LPVOID pvRef, DWORD dwFlags)
	{
		return m_device->EnumEffectsInFile(lpszFileName, pec, pvRef, dwFlags);
	}

	HRESULT MyDirectInputDevice8A::WriteEffectToFile(LPCSTR lpszFileName, DWORD dwEntries, LPDIFILEEFFECT rgDiFileEft, DWORD dwFlags)
	{
		return m_device->WriteEffectToFile(lpszFileName, dwEntries, rgDiFileEft, dwFlags);
	}

	HRESULT MyDirectInputDevice8A::BuildActionMap(LPDIACTIONFORMATA lpdiaf, LPCSTR lpszUserName, DWORD dwFlags)
	{
		return m_device->BuildActionMap(lpdiaf, lpszUserName, dwFlags);
	}

	HRESULT MyDirectInputDevice8A::SetActionMap(LPDIACTIONFORMATA lpdiActionFormat, LPCSTR lptszUserName, DWORD dwFlags)
	{
		return m_device->SetActionMap(lpdiActionFormat, lptszUserName, dwFlags);
	}

	HRESULT MyDirectInputDevice8A::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERA lpdiDevImageInfoHeader)
	{
		return m_device->GetImageInfo(lpdiDevImageInfoHeader);
	}
}

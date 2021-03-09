#pragma once

#include "Th10Hook/Common.h"

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

namespace th
{
	class MyDirectInput8A;

	class MyDirectInputDevice8A :
		public IDirectInputDevice8A
	{
	public:
		MyDirectInputDevice8A(IDirectInputDevice8A* device, MyDirectInput8A* di8);

		/*** IUnknown methods ***/
		STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID* ppvObj) override;
		STDMETHOD_(ULONG, AddRef)(THIS) override;
		STDMETHOD_(ULONG, Release)(THIS) override;

		/*** IDirectInputDevice8A methods ***/
		STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS) override;
		STDMETHOD(EnumObjects)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKA, LPVOID, DWORD) override;
		STDMETHOD(GetProperty)(THIS_ REFGUID, LPDIPROPHEADER) override;
		STDMETHOD(SetProperty)(THIS_ REFGUID, LPCDIPROPHEADER) override;
		STDMETHOD(Acquire)(THIS) override;
		STDMETHOD(Unacquire)(THIS) override;
		STDMETHOD(GetDeviceState)(THIS_ DWORD, LPVOID) override;
		STDMETHOD(GetDeviceData)(THIS_ DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD) override;
		STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT) override;
		STDMETHOD(SetEventNotification)(THIS_ HANDLE) override;
		STDMETHOD(SetCooperativeLevel)(THIS_ HWND, DWORD) override;
		STDMETHOD(GetObjectInfo)(THIS_ LPDIDEVICEOBJECTINSTANCEA, DWORD, DWORD) override;
		STDMETHOD(GetDeviceInfo)(THIS_ LPDIDEVICEINSTANCEA) override;
		STDMETHOD(RunControlPanel)(THIS_ HWND, DWORD) override;
		STDMETHOD(Initialize)(THIS_ HINSTANCE, DWORD, REFGUID) override;
		STDMETHOD(CreateEffect)(THIS_ REFGUID, LPCDIEFFECT, LPDIRECTINPUTEFFECT*, LPUNKNOWN) override;
		STDMETHOD(EnumEffects)(THIS_ LPDIENUMEFFECTSCALLBACKA, LPVOID, DWORD) override;
		STDMETHOD(GetEffectInfo)(THIS_ LPDIEFFECTINFOA, REFGUID) override;
		STDMETHOD(GetForceFeedbackState)(THIS_ LPDWORD) override;
		STDMETHOD(SendForceFeedbackCommand)(THIS_ DWORD) override;
		STDMETHOD(EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK, LPVOID, DWORD) override;
		STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE) override;
		STDMETHOD(Poll)(THIS) override;
		STDMETHOD(SendDeviceData)(THIS_ DWORD, LPCDIDEVICEOBJECTDATA, LPDWORD, DWORD) override;
		STDMETHOD(EnumEffectsInFile)(THIS_ LPCSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD) override;
		STDMETHOD(WriteEffectToFile)(THIS_ LPCSTR, DWORD, LPDIFILEEFFECT, DWORD) override;
		STDMETHOD(BuildActionMap)(THIS_ LPDIACTIONFORMATA, LPCSTR, DWORD) override;
		STDMETHOD(SetActionMap)(THIS_ LPDIACTIONFORMATA, LPCSTR, DWORD) override;
		STDMETHOD(GetImageInfo)(THIS_ LPDIDEVICEIMAGEINFOHEADERA) override;

	private:
		IDirectInputDevice8A* m_device;
		MyDirectInput8A* m_di8;
	};
}

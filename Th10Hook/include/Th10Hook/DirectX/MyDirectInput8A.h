#pragma once

#include "Th10Hook/Common.h"

#include <dinput.h>

namespace th
{
	class MyDirectInput8A :
		public IDirectInput8A
	{
	public:
		MyDirectInput8A(IDirectInput8A* di8);

		/*** IUnknown methods ***/
		STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID* ppvObj) override;
		STDMETHOD_(ULONG, AddRef)(THIS) override;
		STDMETHOD_(ULONG, Release)(THIS) override;

		/*** IDirectInput8A methods ***/
		STDMETHOD(CreateDevice)(THIS_ REFGUID, LPDIRECTINPUTDEVICE8A*, LPUNKNOWN) override;
		STDMETHOD(EnumDevices)(THIS_ DWORD, LPDIENUMDEVICESCALLBACKA, LPVOID, DWORD) override;
		STDMETHOD(GetDeviceStatus)(THIS_ REFGUID) override;
		STDMETHOD(RunControlPanel)(THIS_ HWND, DWORD) override;
		STDMETHOD(Initialize)(THIS_ HINSTANCE, DWORD) override;
		STDMETHOD(FindDevice)(THIS_ REFGUID, LPCSTR, LPGUID) override;
		STDMETHOD(EnumDevicesBySemantics)(THIS_ LPCSTR, LPDIACTIONFORMATA, LPDIENUMDEVICESBYSEMANTICSCBA, LPVOID, DWORD) override;
		STDMETHOD(ConfigureDevices)(THIS_ LPDICONFIGUREDEVICESCALLBACK, LPDICONFIGUREDEVICESPARAMSA, DWORD, LPVOID) override;

	private:
		IDirectInput8A* m_di8;
	};
}

#pragma once

#include <d3d11.h>
#include <atlbase.h>

namespace gc
{
	class Direct3D
	{
	public:
		Direct3D();

		CComPtr<ID3D11Device> getDevice() const;
		CComPtr<ID3D11DeviceContext> getDeviceContext() const;

	private:
		CComPtr<ID3D11Device> m_device;
		CComPtr<ID3D11DeviceContext> m_deviceContext;
	};
}

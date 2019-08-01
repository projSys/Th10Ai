#include "GraphCap/Common.h"
#include "GraphCap/Direct3D.h"

namespace gc
{
	Direct3D::Direct3D()
	{
		// Driver types supported
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = _ARRAYSIZE(driverTypes);

		// Feature levels supported
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			//D3D_FEATURE_LEVEL_12_1,
			//D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};
		UINT numFeatureLevels = _ARRAYSIZE(featureLevels);

		// Create device
		HRESULT hr;
		D3D_FEATURE_LEVEL featureLevel;
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
		{
			hr = D3D11CreateDevice(nullptr, driverTypes[driverTypeIndex], nullptr, 0, featureLevels,
				numFeatureLevels, D3D11_SDK_VERSION, &m_device, &featureLevel, &m_deviceContext);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);
	}

	CComPtr<ID3D11Device> Direct3D::getDevice() const
	{
		return m_device;
	}

	CComPtr<ID3D11DeviceContext> Direct3D::getDeviceContext() const
	{
		return m_deviceContext;
	}
}

#include "TH10Bot/Common.h"
#include "TH10Bot/DesktopImageCV.h"

namespace th
{
	void DesktopImageCV::convertFromD3D11Texture2D(CComPtr<ID3D11DeviceContext> deviceContext,
		CComPtr<ID3D11Texture2D> texture2D)
	{
		cv::directx::convertFromD3D11Texture2D(texture2D, m_data);
	}
}

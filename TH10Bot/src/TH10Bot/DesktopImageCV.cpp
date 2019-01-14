#include "TH10Bot/Common.h"
#include "TH10Bot/DesktopImageCV.h"

namespace th
{
	void DesktopImageCV::convertFromGPU(CComPtr<ID3D11Texture2D> texture2D)
	{
		cv::directx::convertFromD3D11Texture2D(texture2D, m_data);
	}
}

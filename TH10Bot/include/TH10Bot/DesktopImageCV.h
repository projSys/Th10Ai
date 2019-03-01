#pragma once

#include <opencv2/core/directx.hpp>
#include <GraphCap/DesktopImage.h>

namespace th
{
	class DesktopImageCV :
		public DesktopImage
	{
	public:
		virtual void convertFromD3D11Texture2D(CComPtr<ID3D11DeviceContext> deviceContext,
			CComPtr<ID3D11Texture2D> texture2D) override;

	//private:
		cv::Mat m_data;
	};
}

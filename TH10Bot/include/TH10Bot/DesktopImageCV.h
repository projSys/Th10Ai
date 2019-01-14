#pragma once

#include <opencv2/core/directx.hpp>
#include <GraphCap/DesktopImage.h>

namespace th
{
	class DesktopImageCV :
		public DesktopImage
	{
	public:
		virtual ~DesktopImageCV() = default;

		virtual void convertFromGPU(CComPtr<ID3D11Texture2D> texture2D) override;

	//private:
		cv::Mat m_data;
	};
}

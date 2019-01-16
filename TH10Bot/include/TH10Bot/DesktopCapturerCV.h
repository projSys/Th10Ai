#pragma once

#include <opencv2/core/directx.hpp>
#include <GraphCap/DesktopCapturer.h>

namespace th
{
	class DesktopCapturerCV :
		public DesktopCapturerGPU
	{
	public:
		explicit DesktopCapturerCV(UINT output = 0);
		virtual ~DesktopCapturerCV() = default;

	private:
		cv::ocl::Context m_oclContext;
	};
}

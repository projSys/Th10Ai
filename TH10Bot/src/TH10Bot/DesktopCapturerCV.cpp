#include "TH10Bot/Common.h"
#include "TH10Bot/DesktopCapturerCV.h"

namespace th
{
	DesktopCapturerCV::DesktopCapturerCV(Direct3D& d3d) :
		DesktopCapturerGPU(d3d)
	{
		// initialize OpenCL context of OpenCV lib from DirectX
		m_oclContext = cv::directx::ocl::initializeContextFromD3D11Device(m_device);
	}
}

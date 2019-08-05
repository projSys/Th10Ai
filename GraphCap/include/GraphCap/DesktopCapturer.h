#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
#include <atlbase.h>
#include <Base/Size.h>
#include <Base/Rect.h>

#include "GraphCap/Direct3D.h"
#include "GraphCap/DesktopImage.h"

namespace gc
{
	// Win8 or later
	class DesktopCapturer
	{
	public:
		DesktopCapturer(Direct3D& d3d);
		virtual ~DesktopCapturer() = default;

		bool capture(DesktopImage& image, const Rect& rect, time_t timeout = 17);
		Size getDesktopSize() const;

	protected:
		virtual bool doCapture(DesktopImage& image, const Rect& rect,
			CComPtr<ID3D11Texture2D> acquiredDesktopImage)/* = 0*/; // Python无法导出

		CComPtr<ID3D11Device> m_device;
		CComPtr<ID3D11DeviceContext> m_deviceContext;
		DXGI_OUTPUT_DESC m_outputDesc;
		CComPtr<IDXGIOutputDuplication> m_desktopDupl;
	};

	class DesktopCapturerGPU :
		public DesktopCapturer
	{
	public:
		DesktopCapturerGPU(Direct3D& d3d);

	protected:
		virtual bool doCapture(DesktopImage& image, const Rect& rect,
			CComPtr<ID3D11Texture2D> acquiredDesktopImage) override;

	private:
		CComPtr<ID3D11Texture2D> m_buffer;
		Size m_size;
	};

	class DesktopCapturerCPU :
		public DesktopCapturer
	{
	public:
		DesktopCapturerCPU(Direct3D& d3d);

	protected:
		virtual bool doCapture(DesktopImage& image, const Rect& rect,
			CComPtr<ID3D11Texture2D> acquiredDesktopImage) override;

	private:
		CComPtr<ID3D11Texture2D> m_buffer;
		Size m_size;
	};
}

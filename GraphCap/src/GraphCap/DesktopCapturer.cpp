#include "GraphCap/Common.h"
#include "GraphCap/DesktopCapturer.h"

#include <Base/ScopeGuard.h>

namespace gc
{
	DesktopCapturer::DesktopCapturer(Direct3D& d3d) :
		m_device(d3d.getDevice()),
		m_deviceContext(d3d.getDeviceContext())
	{
		// Get DXGI device
		CComPtr<IDXGIDevice> dxgiDevice;
		HRESULT hr = m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		// Get DXGI adapter
		CComPtr<IDXGIAdapter> dxgiAdapter;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
		dxgiDevice = nullptr;
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		// Get output
		CComPtr<IDXGIOutput> dxgiOutput;
		hr = dxgiAdapter->EnumOutputs(0, &dxgiOutput);
		dxgiAdapter = nullptr;
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		m_outputDesc = {};
		hr = dxgiOutput->GetDesc(&m_outputDesc);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		// QI for Output 1
		CComPtr<IDXGIOutput1> dxgiOutput1;
		hr = dxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&dxgiOutput1));
		dxgiOutput = nullptr;
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		// Create desktop duplication
		hr = dxgiOutput1->DuplicateOutput(m_device, &m_desktopDupl);
		dxgiOutput1 = nullptr;
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);
	}

	bool DesktopCapturer::capture(DesktopImage& image, const Rect& rect, time_t timeout)
	{
		// Get new frame
		DXGI_OUTDUPL_FRAME_INFO frameInfo = {};
		CComPtr<IDXGIResource> desktopResource;
		HRESULT hr = m_desktopDupl->AcquireNextFrame(static_cast<UINT>(timeout), &frameInfo, &desktopResource);
		// 桌面没有变化会超时
		if (hr == DXGI_ERROR_WAIT_TIMEOUT)
			return false;
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);
		ON_SCOPE_EXIT([&]()
		{
			m_desktopDupl->ReleaseFrame();
		});

		// QI for IDXGIResource
		CComPtr<ID3D11Texture2D> acquiredDesktopImage;
		hr = desktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&acquiredDesktopImage));
		desktopResource = nullptr;
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		return doCapture(image, rect, acquiredDesktopImage);
	}

	bool DesktopCapturer::doCapture(DesktopImage& image, const Rect& rect,
		CComPtr<ID3D11Texture2D> acquiredDesktopImage)
	{
		THROW_BASE_EXCEPTION(Exception() << err_str("Unimplemented function."));
		return false;
	}

	Size DesktopCapturer::getDesktopSize() const
	{
		return Size(m_outputDesc.DesktopCoordinates.right - m_outputDesc.DesktopCoordinates.left,
			m_outputDesc.DesktopCoordinates.bottom - m_outputDesc.DesktopCoordinates.top);
	}



	DesktopCapturerGPU::DesktopCapturerGPU(Direct3D& d3d) :
		DesktopCapturer(d3d)
	{
	}

	bool DesktopCapturerGPU::doCapture(DesktopImage& image, const Rect& rect,
		CComPtr<ID3D11Texture2D> acquiredDesktopImage)
	{
		if (rect.isEmpty())
		{
			image.convertFromD3D11Texture2D(m_deviceContext, acquiredDesktopImage);
		}
		else
		{
			D3D11_TEXTURE2D_DESC desktopDesc = {};
			acquiredDesktopImage->GetDesc(&desktopDesc);
			Rect desktopRect(0, 0, desktopDesc.Width, desktopDesc.Height);
			// 窗口位置超出桌面范围
			if ((rect & desktopRect) != rect)
				return false;

			if (m_size != rect.getSize())
				m_buffer = nullptr;

			if (m_buffer == nullptr)
			{
				D3D11_TEXTURE2D_DESC bufferDesc = desktopDesc;
				bufferDesc.Width = static_cast<UINT>(rect.width);
				bufferDesc.Height = static_cast<UINT>(rect.height);
				bufferDesc.BindFlags = 0;
				bufferDesc.MiscFlags = 0;
				HRESULT hr = m_device->CreateTexture2D(&bufferDesc, nullptr, &m_buffer);
				if (FAILED(hr))
					THROW_DIRECTX_HRESULT(hr);

				m_size = rect.getSize();
			}

			D3D11_BOX box = {};
			box.left = static_cast<UINT>(rect.x);
			box.top = static_cast<UINT>(rect.y);
			box.front = 0;
			box.right = static_cast<UINT>(rect.x + rect.width);
			box.bottom = static_cast<UINT>(rect.y + rect.height);
			box.back = 1;
			m_deviceContext->CopySubresourceRegion(m_buffer, 0, 0, 0, 0, acquiredDesktopImage, 0, &box);

			image.convertFromD3D11Texture2D(m_deviceContext, m_buffer);
		}
		return true;
	}



	DesktopCapturerCPU::DesktopCapturerCPU(Direct3D& d3d) :
		DesktopCapturer(d3d)
	{
	}

	bool DesktopCapturerCPU::doCapture(DesktopImage& image, const Rect& rect,
		CComPtr<ID3D11Texture2D> acquiredDesktopImage)
	{
		if (rect.isEmpty())
		{
			D3D11_TEXTURE2D_DESC desktopDesc = {};
			acquiredDesktopImage->GetDesc(&desktopDesc);
			Rect desktopRect(0, 0, desktopDesc.Width, desktopDesc.Height);
			if (m_size != desktopRect.getSize())
				m_buffer = nullptr;

			if (m_buffer == nullptr)
			{
				D3D11_TEXTURE2D_DESC bufferDesc = desktopDesc;
				bufferDesc.Usage = D3D11_USAGE_STAGING;
				bufferDesc.BindFlags = 0;
				bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				bufferDesc.MiscFlags = 0;
				HRESULT hr = m_device->CreateTexture2D(&bufferDesc, nullptr, &m_buffer);
				if (FAILED(hr))
					THROW_DIRECTX_HRESULT(hr);

				m_size = desktopRect.getSize();
			}

			m_deviceContext->CopyResource(m_buffer, acquiredDesktopImage);

			image.convertFromD3D11Texture2D(m_deviceContext, m_buffer);
		}
		else
		{
			D3D11_TEXTURE2D_DESC desktopDesc = {};
			acquiredDesktopImage->GetDesc(&desktopDesc);
			Rect desktopRect(0, 0, desktopDesc.Width, desktopDesc.Height);
			// 窗口位置超出桌面范围
			if ((rect & desktopRect) != rect)
				return false;

			if (m_size != rect.getSize())
				m_buffer = nullptr;

			if (m_buffer == nullptr)
			{
				D3D11_TEXTURE2D_DESC bufferDesc = desktopDesc;
				bufferDesc.Width = static_cast<UINT>(rect.width);
				bufferDesc.Height = static_cast<UINT>(rect.height);
				bufferDesc.Usage = D3D11_USAGE_STAGING;
				bufferDesc.BindFlags = 0;
				bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
				bufferDesc.MiscFlags = 0;
				HRESULT hr = m_device->CreateTexture2D(&bufferDesc, nullptr, &m_buffer);
				if (FAILED(hr))
					THROW_DIRECTX_HRESULT(hr);

				m_size = rect.getSize();
			}

			D3D11_BOX box = {};
			box.left = static_cast<UINT>(rect.x);
			box.top = static_cast<UINT>(rect.y);
			box.front = 0;
			box.right = static_cast<UINT>(rect.x + rect.width);
			box.bottom = static_cast<UINT>(rect.y + rect.height);
			box.back = 1;
			m_deviceContext->CopySubresourceRegion(m_buffer, 0, 0, 0, 0, acquiredDesktopImage, 0, &box);

			image.convertFromD3D11Texture2D(m_deviceContext, m_buffer);
		}
		return true;
	}
}

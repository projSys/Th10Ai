#include "GraphCap/Common.h"
#include "GraphCap/Renderer.h"

#include <Base/ScopeGuard.h>

namespace gc
{
	Renderer::Renderer(Direct3D& d3d, Window& window) :
		m_device(d3d.getDevice()),
		m_deviceContext(d3d.getDeviceContext()),
		m_occlusionCookie(0)
	{
		// Get DXGI factory
		CComPtr<IDXGIDevice> dxgiDevice;
		HRESULT hr = m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		CComPtr<IDXGIAdapter> dxgiAdapter;
		hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
		dxgiDevice = nullptr;
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&m_factory2));
		dxgiAdapter = nullptr;
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		// Register for occlusion status windows message
		hr = m_factory2->RegisterOcclusionStatusWindow(window, OCCLUSION_STATUS_MSG, &m_occlusionCookie);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		// Get window size
		Rect rect = window.getClientRect();

		// Create swapchain for window
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = static_cast<UINT>(rect.width);
		swapChainDesc.Height = static_cast<UINT>(rect.height);
		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		hr = m_factory2->CreateSwapChainForHwnd(m_device, window, &swapChainDesc, nullptr, nullptr, &m_swapChain1);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);

		// Disable the ALT-ENTER shortcut for entering full-screen mode
		hr = m_factory2->MakeWindowAssociation(window, DXGI_MWA_NO_ALT_ENTER);
		if (FAILED(hr))
			THROW_DIRECTX_HRESULT(hr);
	}
}

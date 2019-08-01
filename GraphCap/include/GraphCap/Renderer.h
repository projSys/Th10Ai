#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
#include <atlbase.h>
#include <Base/Window.h>

#include "GraphCap/Direct3D.h"

namespace gc
{
#define OCCLUSION_STATUS_MSG WM_USER

	class Renderer
	{
	public:
		Renderer(Direct3D& d3d, Window& window);

	private:
		CComPtr<ID3D11Device> m_device;
		CComPtr<ID3D11DeviceContext> m_deviceContext;
		CComPtr<IDXGIFactory2> m_factory2;
		DWORD m_occlusionCookie;
		CComPtr<IDXGISwapChain1> m_swapChain1;
	};
}

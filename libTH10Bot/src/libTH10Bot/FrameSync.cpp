#include "libTH10Bot/Common.h"
#include "libTH10Bot/FrameSync.h"

namespace th
{
	FrameSync::FrameSync() :
		m_presentReadied(false),
		m_d3d9Hook(this)
	{
	}

	void FrameSync::onPresentBegin(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		notifyPresent();
	}

	void FrameSync::onPresentEnd(HRESULT& hr, IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
	}

	void FrameSync::enable(bool enabled)
	{
		m_d3d9Hook.enable(enabled);
	}

	void FrameSync::notifyPresent()
	{
		std::unique_lock<std::mutex> lock(m_presentMutex);
		m_presentReadied = true;
		m_presentCond.notify_one();
	}

	void FrameSync::waitPresent()
	{
		bool waited = false;
		std::unique_lock<std::mutex> lock(m_presentMutex);
		while (!m_presentReadied)
		{
			m_presentCond.wait(lock);
			waited = true;
		}
		m_presentReadied = false;
		if (!waited)
			std::cout << "读取不及时。" << std::endl;
	}
}

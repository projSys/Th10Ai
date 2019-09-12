#include "libTH10AI/Common.h"
#include "libTH10AI/FrameSync.h"

#include "libTH10AI/Input.h"

namespace th
{
	FrameSync::FrameSync() :
		m_presentReadied(false),
		m_d3d9Hook(this)
	{
	}

	std::chrono::steady_clock::time_point g_presentBeginTime;
	std::chrono::steady_clock::time_point g_presentEndTime;

	void FrameSync::onPresentBegin(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		// ��Ϸ�߼��Ĵ���ʱ��
		g_presentBeginTime = std::chrono::steady_clock::now();
		//std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
		//	g_presentBeginTime - g_getDeviceStateTime);
		//std::cout << interval.count() << " ";

		notifyPresent();
	}

	void FrameSync::onPresentEnd(HRESULT& hr, IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		// ��ֱͬ���ĵȴ�ʱ��
		g_presentEndTime = std::chrono::steady_clock::now();
		//interval = std::chrono::duration_cast<std::chrono::milliseconds>(
		//	g_presentEndTime - g_presentBeginTime);
		//std::cout << interval.count() << std::endl;
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

	bool FrameSync::waitPresent()
	{
		bool waited = false;
		std::unique_lock<std::mutex> lock(m_presentMutex);
		while (!m_presentReadied)
		{
			m_presentCond.wait(lock);
			waited = true;
		}
		m_presentReadied = false;
		return waited;
	}
}

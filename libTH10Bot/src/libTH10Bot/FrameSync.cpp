#include "libTH10Bot/Common.h"
#include "libTH10Bot/FrameSync.h"

#include <thread>

#include "libTH10Bot/Input.h"

namespace th
{
	FrameSync::FrameSync() :
		m_presentReadied(false),
		m_d3d9Hook(this)
	{
	}

	std::chrono::steady_clock::time_point g_presentBeginTime;
	std::chrono::steady_clock::time_point g_presentEndTime;
	time_t g_presentTimespan;

	void FrameSync::onPresentBegin(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		// 游戏逻辑的处理时间
		g_presentBeginTime = std::chrono::steady_clock::now();
		//std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
		//	g_presentBeginTime - g_getDeviceStateTime);
		//std::cout << interval.count() << " ";

		notifyPresent();
	}

	void FrameSync::onPresentEnd(HRESULT& hr, IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
		HWND destWindowOverride, CONST RGNDATA* dirtyRegion)
	{
		if (FAILED(hr))
		{
			std::cout << "Present()失败。" << std::endl;
			return;
		}

		// 垂直同步的等待时间
		g_presentEndTime = std::chrono::steady_clock::now();
		std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
			g_presentEndTime - g_presentBeginTime);
		//std::cout << interval.count() << std::endl;
		g_presentTimespan = interval.count();

		// 如果垂直同步的等待时间小于5毫秒，则延时一下，会导致掉帧
		if (g_presentTimespan < 5)
		{
			time_t delayTimespan = 5 - g_presentTimespan;
			std::cout << "作弊延时: " << delayTimespan << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(delayTimespan));
		}
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
		{
			std::chrono::steady_clock::time_point waitTime = std::chrono::steady_clock::now();
			std::chrono::milliseconds interval = std::chrono::duration_cast<std::chrono::milliseconds>(
				waitTime - g_presentBeginTime);
			std::cout << "读取数据延迟了：" << interval.count() << std::endl;
		}
	}
}

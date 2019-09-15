#pragma once

#include <mutex>
#include <condition_variable>

#include "libTH10Bot/ApiHook/D3D9Hook.h"

namespace th
{
	class FrameSync :
		private D3D9Listener
	{
	public:
		FrameSync();

		void enable(bool enabled);

		void notifyPresent();
		void waitPresent();

	private:
		virtual void onPresentBegin(IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion) override;
		virtual void onPresentEnd(HRESULT& hr, IDirect3DDevice9* device, CONST RECT* sourceRect, CONST RECT* destRect,
			HWND destWindowOverride, CONST RGNDATA* dirtyRegion) override;

		std::mutex m_presentMutex;
		std::condition_variable m_presentCond;
		bool m_presentReadied;

		D3D9Hook m_d3d9Hook;
	};

	extern std::chrono::steady_clock::time_point g_presentBeginTime;
	extern std::chrono::steady_clock::time_point g_presentEndTime;
	extern time_t g_presentTimespan;
}

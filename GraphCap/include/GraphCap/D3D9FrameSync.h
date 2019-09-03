#pragma once

#include <chrono>
#include <thread>
#include <atomic>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

#include "GraphCap/FrameListener.h"

namespace gc
{
	namespace bip = boost::interprocess;

	struct D3D9FSSharedData
	{
		bip::interprocess_mutex presentBeginMutex;
		bip::interprocess_condition presentBeginCond;
		bool presentBeginReady;
		bip::interprocess_mutex presentEndMutex;
		bip::interprocess_condition presentEndCond;
		bool presentEndReady;
		std::chrono::steady_clock::time_point m_t0;
	};

	class D3D9FrameSync
	{
	public:
		D3D9FrameSync(FrameListener* listener);
		~D3D9FrameSync();

		bool waitForPresent(time_t timeout);
		bool waitForPresent();

	private:
		void syncProc();
		void syncProc2();

		bip::managed_shared_memory m_memory;
		D3D9FSSharedData* m_data;
		FrameListener* m_listener;
		std::thread m_thread;
		std::atomic_bool m_done;
		std::thread m_thread2;
		std::atomic_bool m_done2;
	};
}

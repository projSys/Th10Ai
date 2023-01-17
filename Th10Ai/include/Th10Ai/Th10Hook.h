#pragma once

#include "Th10Ai/Common.h"

#include <thread>
#include <boost/process.hpp>
#include <Th10Base/SharedMemory.h>
#include <Th10Base/SharedData.h>

namespace th
{
	namespace bp = boost::process;

	class Config;

	class Th10Hook
	{
	public:
		Th10Hook();
		~Th10Hook();

		void launch(const Config& config);

		bool isActive() const;
		void setActive(bool active);
		HWND getWindow() const;
		bool waitUpdate();
		bool waitUpdate(const Time& timeout);
		bool isExit() const;
		const SharedStatus& getReadableStatus() const;
		void notifyInput();
		SharedInput& getWritableInput();

	private:
		void th10Proc();

	private:
		SharedMemory m_sharedMemory;
		SharedData* m_sharedData;

		bp::child m_th10;
		bp::ipstream m_ips;
		std::thread m_th10Thread;
	};
}

#pragma once

#include "Th10Ai/Common.h"

#include <filesystem>
#include <thread>
#include <boost/process.hpp>
#include <Base/Time.h>
#include <Th10Base/SharedMemory.h>
#include <Th10Base/SharedData.h>

namespace th
{
	namespace fs = std::filesystem;
	namespace bp = boost::process;

	class Th10Hook
	{
	public:
		Th10Hook();
		~Th10Hook();

		void launch(const fs::path& exePath);

		bool isActive() const;
		void setActive(bool active);
		HWND getWindow() const;

		bool waitUpdate();
		const SharedStatus& getReadableStatus() const;

		void notifyInput();
		SharedInput& getWritableInput();

	private:
		void stdioProc();

		bool waitInit(const Time& timeout = Time(3000));

	private:
		SharedMemory m_sharedMemory;
		SharedData* m_sharedData;

		bp::child m_th10;
		bp::ipstream m_ips;
		std::thread m_stdioThread;
	};
}

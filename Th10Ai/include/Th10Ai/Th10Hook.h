#pragma once

#include "Th10Ai/Common.h"

#include <thread>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <Th10Base/SharedMemory.h>
#include <Th10Base/SharedData.h>

namespace th
{
	namespace bp = boost::process;
	namespace ba = boost::asio;
	namespace bs = boost::system;

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
		void asioProc();
		void doStdoutRead();
		void doStderrRead();

	private:
		static constexpr uint_t MEMORY_SIZE = 32 * 1024 * 1024;
		static constexpr uint_t BUFFER_SIZE = 2 * 1024;

		SharedMemory m_sharedMemory;
		SharedData* m_sharedData;

		bp::child m_th10;
		std::thread m_asioThread;
		ba::io_context m_ioc;
		bp::async_pipe m_apOut;
		bp::async_pipe m_apErr;
		char m_bufOut[BUFFER_SIZE];
		char m_bufErr[BUFFER_SIZE];
	};
}

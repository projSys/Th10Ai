#pragma once

#include "Base/Common.h"

#include <filesystem>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>

namespace base
{
	namespace fs = std::filesystem;
	namespace blog = boost::log;
	namespace sinks = boost::log::sinks;

	//TODO: 重写辣鸡log
	class Logger
	{
	public:
		Logger();

		void addCommonAttributes();
		void addFileLog(const fs::path& filePath);
		void flush();

	private:
		boost::shared_ptr<sinks::synchronous_sink<
			sinks::text_file_backend>> m_fileSink;
	};

	extern Logger g_logger;
}

#include "Base/Log.h"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace base
{
	namespace log = boost::log;

	Log::Log() :
		Singleton(this)
	{
	}

	void Log::addFileLog(const fs::path& filePath)
	{
		log::add_file_log
		(
			log::keywords::file_name = filePath,
			log::keywords::rotation_size = 8 * 1024 * 1024,
			log::keywords::time_based_rotation = log::sinks::file::rotation_at_time_point(0, 0, 0),
			log::keywords::format = "%Message%"
		);
	}

	void Log::logTrace(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(trace) << str << std::endl;
	}

	void Log::logDebug(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(debug) << str << std::endl;
	}

	void Log::logInfo(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(info) << str << std::endl;
	}

	void Log::logWarning(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(warning) << str << std::endl;
	}

	void Log::logError(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(error) << str << std::endl;
	}

	void Log::logFatal(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(fatal) << str << std::endl;
	}
}

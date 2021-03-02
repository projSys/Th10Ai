#include "Base/Logger.h"

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace base
{
	namespace log = boost::log;

	Logger::Logger() :
		Singleton(this)
	{
	}

	void Logger::addFileLog(const std::string& filePath)
	{
		log::add_file_log
		(
			log::keywords::file_name = filePath,
			log::keywords::rotation_size = 8 * 1024 * 1024,
			log::keywords::time_based_rotation = log::sinks::file::rotation_at_time_point(0, 0, 0),
			log::keywords::format = "%Message%"
		);
	}

	void Logger::logTrace(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(trace) << str;
	}

	void Logger::logDebug(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(debug) << str;
	}

	void Logger::logInfo(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(info) << str;
	}

	void Logger::logWarning(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(warning) << str;
	}

	void Logger::logError(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(error) << str;
	}

	void Logger::logFatal(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(fatal) << str;
	}
}

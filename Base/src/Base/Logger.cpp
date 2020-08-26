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

	void Logger::addFileLog(const std::string& filename)
	{
		log::add_file_log
		(
			log::keywords::file_name = filename,
			log::keywords::rotation_size = 8 * 1024 * 1024,
			log::keywords::time_based_rotation = log::sinks::file::rotation_at_time_point(0, 0, 0),
			log::keywords::format = "%Message%"
		);
	}

	void Logger::trace(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(trace) << str;
	}

	void Logger::debug(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(debug) << str;
	}

	void Logger::info(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(info) << str;
	}

	void Logger::warning(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(warning) << str;
	}

	void Logger::error(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(error) << str;
	}

	void Logger::fatal(const std::string& str)
	{
		BOOST_LOG_TRIVIAL(fatal) << str;
	}
}

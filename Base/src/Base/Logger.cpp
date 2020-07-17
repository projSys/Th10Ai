#include "Base/Common.h"
#include "Base/Logger.h"

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace base
{
	using namespace boost::log;

	Logger::Logger() :
		Singleton(this)
	{
	}

	void Logger::addFileLog(const string& filename)
	{
		add_file_log
		(
			keywords::file_name = filename,
			keywords::rotation_size = 8 * 1024 * 1024,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::format = "%Message%"
		);
	}

	void Logger::trace(const string& str)
	{
		BOOST_LOG_TRIVIAL(trace) << str;
	}

	void Logger::debug(const string& str)
	{
		BOOST_LOG_TRIVIAL(debug) << str;
	}

	void Logger::info(const string& str)
	{
		BOOST_LOG_TRIVIAL(info) << str;
	}

	void Logger::warning(const string& str)
	{
		BOOST_LOG_TRIVIAL(warning) << str;
	}

	void Logger::error(const string& str)
	{
		BOOST_LOG_TRIVIAL(error) << str;
	}

	void Logger::fatal(const string& str)
	{
		BOOST_LOG_TRIVIAL(fatal) << str;
	}
}

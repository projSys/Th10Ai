#include "Base/Log.h"

#include <ostream>
#include <iostream>
#include <fstream>
#include <boost/core/null_deleter.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/log/detail/default_attribute_names.hpp>
#include <boost/log/attributes/clock.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp> 

#include "Base/Traits.h"

namespace base
{
	namespace aux = boost::log::aux;

	std::ostream& operator <<(std::ostream& os, SeverityLevel level)
	{
		static const char* descs[] =
		{
			"DEBUG",
			"INFO",
			"WARNING",
			"ERROR",
			"FATAL"
		};
		os << descs[to_underlying(level)];
		return os;
	}

	BOOST_LOG_GLOBAL_LOGGER_INIT(Logger, src::severity_logger_mt<SeverityLevel>)
	{
		src::severity_logger_mt<SeverityLevel> logger;
		logger.add_attribute(aux::default_attribute_names::timestamp(), attrs::local_clock());
		return logger;
	}

	void Log::Initialize(const fs::path& logPath)
	{
		logging::core_ptr core = logging::core::get();

		using Sink = sinks::synchronous_sink<sinks::text_ostream_backend>;
		boost::shared_ptr<Sink> sink = boost::make_shared<Sink>();
		core->add_sink(sink);

		sink->set_formatter
		(
			expr::stream
			<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
			<< " [" << expr::attr<SeverityLevel>("Severity") << "] "
			<< expr::smessage
		);

		Sink::locked_backend_ptr backend = sink->locked_backend();
		boost::shared_ptr<std::ostream> ofs = boost::make_shared<std::ofstream>(logPath.native(), std::ios::app);
		backend->add_stream(ofs);
		boost::shared_ptr<std::ostream> clog(&std::clog, boost::null_deleter());
		backend->add_stream(clog);
	}

	void Log::Flush()
	{
		logging::core_ptr core = logging::core::get();
		core->flush();
	}
}

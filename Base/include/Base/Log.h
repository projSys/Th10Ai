#pragma once

#include "Base/Common.h"

#include <iosfwd>
#include <filesystem>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include "Base/Types.h"

namespace base
{
	namespace fs = std::filesystem;
	namespace logging = boost::log;
	namespace src = boost::log::sources;
	namespace sinks = boost::log::sinks;
	namespace keywords = boost::log::keywords;
	namespace attrs = boost::log::attributes;
	namespace expr = boost::log::expressions;

	class Log
	{
	public:
		static void Initialize(const fs::path& filePath);
		static void Flush();
	};

	enum class SeverityLevel : uint_t
	{
		DEBUG,
		INFO,
		WARNING,
		_ERROR,
		FATAL
	};

	std::ostream& operator <<(std::ostream& os, SeverityLevel level);

	BOOST_LOG_GLOBAL_LOGGER(Logger, src::severity_logger_mt<SeverityLevel>)

#define LOG_DEBUG() BOOST_LOG_SEV(::base::Logger::get(), ::base::SeverityLevel::DEBUG)
#define LOG_INFO() BOOST_LOG_SEV(::base::Logger::get(), ::base::SeverityLevel::INFO)
#define LOG_WARNING() BOOST_LOG_SEV(::base::Logger::get(), ::base::SeverityLevel::WARNING)
#define LOG_ERROR() BOOST_LOG_SEV(::base::Logger::get(), ::base::SeverityLevel::_ERROR)
#define LOG_FATAL() BOOST_LOG_SEV(::base::Logger::get(), ::base::SeverityLevel::FATAL)
}

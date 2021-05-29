#pragma once

#include "Base/Common.h"

#include "Base/String.h"
#include "Base/Singleton.h"

namespace boost
{
	namespace filesystem
	{
		class path;
	}
}

namespace base
{
	namespace fs = boost::filesystem;

	class Log :
		public Singleton<Log>
	{
	public:
		Log();

		void addFileLog(const fs::path& filePath);

		void logTrace(const std::string& str);
		void logDebug(const std::string& str);
		void logInfo(const std::string& str);
		void logWarning(const std::string& str);
		void logError(const std::string& str);
		void logFatal(const std::string& str);
	};

#define BASE_LOG_TRACE(str) base::Log::GetInstance().logTrace(str)
#define BASE_LOG_DEBUG(str) base::Log::GetInstance().logDebug(str)
#define BASE_LOG_INFO(str) base::Log::GetInstance().logInfo(str)
#define BASE_LOG_WARNING(str) base::Log::GetInstance().logWarning(str)
#define BASE_LOG_ERROR(str) base::Log::GetInstance().logError(str)
#define BASE_LOG_FATAL(str) base::Log::GetInstance().logFatal(str)
}

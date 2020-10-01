#pragma once

#include "Base/Common.h"

#include "Base/String.h"
#include "Base/Singleton.h"

namespace base
{
	class Logger :
		public Singleton<Logger>
	{
	public:
		Logger();

		void addFileLog(const std::string& fileName);

		void logTrace(const std::string& str);
		void logDebug(const std::string& str);
		void logInfo(const std::string& str);
		void logWarning(const std::string& str);
		void logError(const std::string& str);
		void logFatal(const std::string& str);
	};

#define BASE_LOG_TRACE(str) base::Logger::GetInstance().logTrace(str)
#define BASE_LOG_DEBUG(str) base::Logger::GetInstance().logDebug(str)
#define BASE_LOG_INFO(str) base::Logger::GetInstance().logInfo(str)
#define BASE_LOG_WARNING(str) base::Logger::GetInstance().logWarning(str)
#define BASE_LOG_ERROR(str) base::Logger::GetInstance().logError(str)
#define BASE_LOG_FATAL(str) base::Logger::GetInstance().logFatal(str)
}

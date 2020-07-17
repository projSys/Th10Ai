#pragma once

#include <string>

#include "Base/Singleton.h"

namespace base
{
	class Logger :
		public Singleton<Logger>
	{
	public:
		Logger();

		void addFileLog(const string& filename);

		void trace(const string& str);
		void debug(const string& str);
		void info(const string& str);
		void warning(const string& str);
		void error(const string& str);
		void fatal(const string& str);
	};

#define BASE_LOG_TRACE(str) base::Logger::GetInstance().trace(str)
#define BASE_LOG_DEBUG(str) base::Logger::GetInstance().debug(str)
#define BASE_LOG_INFO(str) base::Logger::GetInstance().info(str)
#define BASE_LOG_WARNING(str) base::Logger::GetInstance().warning(str)
#define BASE_LOG_ERROR(str) base::Logger::GetInstance().error(str)
#define BASE_LOG_FATAL(str) base::Logger::GetInstance().fatal(str)
}

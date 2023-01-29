#include "Base/Logger.h"

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace base
{
	Logger g_logger;

	Logger::Logger()
	{
	}

	void Logger::addCommonAttributes()
	{
		bl::add_common_attributes();
	}

	void Logger::addFileLog(const fs::path& filePath)
	{
		m_fileSink = bl::add_file_log(filePath);
	}

	void Logger::flush()
	{
		if (m_fileSink != nullptr)
			m_fileSink->flush();
	}
}

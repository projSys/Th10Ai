#include "libTh10Ai/Common.h"
#include "libTh10Ai/libTh10Ai.h"

#include <boost/log/utility/setup/file.hpp>

#include "libTh10Ai/DllInject/DllMain.h"
#include "libTh10Ai/Th10Ai.h"

namespace th
{
	namespace bl = boost::log;

	libTh10Ai g_libTh10Ai;

	void WINAPI Th10AiMain()
	{
		std::string logName = Util::GetModuleDir(g_module) + "/Th10Ai_%N.log";
		bl::add_file_log
		(
			bl::keywords::file_name = logName,
			bl::keywords::rotation_size = 10 * 1024 * 1024,
			bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
			bl::keywords::format = "%Message%"
		);

		g_libTh10Ai.main(_T("BASE"), nullptr);
	}

	bool libTh10Ai::onAttach()
	{
		std::string logName = Util::GetModuleDir(g_module) + "/libTh10Ai_%N.log";
		bl::add_file_log
		(
			bl::keywords::file_name = logName,
			bl::keywords::rotation_size = 10 * 1024 * 1024,
			bl::keywords::time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0),
			bl::keywords::format = "%Message%"
		);

		try
		{
			if (m_th10Ai != nullptr)
				THROW_BASE_EXCEPTION(Exception() << err_str(u8"Th10AiÒÑ´´½¨¡£"));

			m_th10Ai = std::make_shared<Th10Ai>();
			return true;
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
			return false;
		}
	}

	void libTh10Ai::onDetach()
	{
		m_th10Ai = nullptr;
	}
}

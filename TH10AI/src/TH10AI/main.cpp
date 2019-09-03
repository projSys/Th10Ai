#include "TH10AI/Common.h"

#include <boost/locale.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <Base/ScopeGuard.h>

namespace blc = boost::locale::conv;
namespace bl = boost::log;

int main()
{
	try
	{
		std::string logName = win::Utils::GetModuleDir() + "/TH10AI.log";
		bl::add_file_log(logName);

		HWND console = GetConsoleWindow();
		HMENU menu = GetSystemMenu(console, FALSE);
		EnableMenuItem(menu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);

		std::string dllName = win::Utils::GetModuleDir() + "/libTH10AI.dll";
		std::wstring dllNameW = blc::utf_to_utf<wchar_t>(dllName);
		HMODULE dll = LoadLibrary(dllNameW.c_str());
		if (dll == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		ON_SCOPE_EXIT([&]()
		{
			FreeLibrary(dll);
		});

		typedef bool(WINAPI *Th10Main_t)();
		Th10Main_t th10Main = reinterpret_cast<Th10Main_t>(GetProcAddress(dll, "Th10Main"));
		if (th10Main == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());
		if (!th10Main())
			THROW_BASE_EXCEPTION(base::Exception() << base::err_str(u8"Th10Main()调用失败，详细信息请查看libTH10AI.log。"));
	}
	catch (...)
	{
		std::string what = boost::current_exception_diagnostic_information();
		BOOST_LOG_TRIVIAL(error) << what;
		std::cout << what << std::endl;
		system("pause");
	}
	return 0;
}

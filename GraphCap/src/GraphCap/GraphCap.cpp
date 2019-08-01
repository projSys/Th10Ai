#include "GraphCap/Common.h"
#include "GraphCap/GraphCap.h"

#include <boost/locale.hpp>

namespace gc
{
	namespace blc = boost::locale::conv;

	GraphCap::GraphCap(Window& target, int_t capType)
	{
		std::string dllName = Utils::GetModuleDir() + "/GraphHook.dll";
		std::wstring dllNameW = blc::utf_to_utf<wchar_t>(dllName);

		m_hookDll = LoadLibrary(dllNameW.c_str());
		if (m_hookDll == nullptr)
			THROW_WINDOWS_ERROR(GetLastError());

		m_hookFunc = reinterpret_cast<Hook_t>(GetProcAddress(m_hookDll, "Hook"));
		if (m_hookFunc == nullptr)
		{
			FreeLibrary(m_hookDll);
			THROW_WINDOWS_ERROR(GetLastError());
		}

		m_unhookFunc = reinterpret_cast<Unhook_t>(GetProcAddress(m_hookDll, "Unhook"));
		if (m_unhookFunc == nullptr)
		{
			FreeLibrary(m_hookDll);
			THROW_WINDOWS_ERROR(GetLastError());
		}

		if (!m_hookFunc(target, capType))
		{
			m_unhookFunc();
			FreeLibrary(m_hookDll);
			THROW_BASE_EXCEPTION(Exception() << err_str(u8"注入GraphHook.dll失败，详细信息请查看GraphHook.log。"));
		}
	}

	GraphCap::~GraphCap()
	{
		m_unhookFunc();
		FreeLibrary(m_hookDll);
	}
}

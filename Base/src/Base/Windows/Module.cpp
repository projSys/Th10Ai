#include "Base/Windows/Module.h"

#include <system_error>

#include "Base/Exception.h"
#include "Base/Windows/Apis.h"

namespace base
{
	namespace win
	{
		Module Module::Get(const char* moduleName)
		{
			std::wstring moduleNameW = Apis::Utf8ToWide(moduleName);

			Module module(GetModuleHandleW(moduleNameW.c_str()));
			if (module == nullptr)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			return module;
		}

		Module::Module() :
			m_module(nullptr)
		{
		}

		Module::Module(HMODULE module) :
			m_module(module)
		{
		}

		Module::~Module()
		{
			//if (m_module != nullptr)
			//	FreeLibrary(m_module);
		}

		fs::path Module::getPath() const
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = GetModuleFileNameW(m_module, buffer, BUFFER_SIZE - 1);
			if (ret == 0)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));

			return fs::path(buffer);
		}

		fs::path Module::getDir() const
		{
			return getPath().remove_filename();
		}

		FARPROC Module::getProcAddress(const char* procName) const
		{
			FARPROC proc = GetProcAddress(m_module, procName);
			if (proc == nullptr)
				BASE_THROW(std::system_error(GetLastError(), std::system_category()));
			return proc;
		}

		bool Module::operator ==(nullptr_t) const
		{
			return m_module == nullptr;
		}

		bool Module::operator !=(nullptr_t) const
		{
			return m_module != nullptr;
		}

		bool Module::operator !() const
		{
			return m_module == nullptr;
		}

		Module::operator bool() const
		{
			return m_module != nullptr;
		}

		Module::operator HMODULE() const
		{
			return m_module;
		}
	}
}

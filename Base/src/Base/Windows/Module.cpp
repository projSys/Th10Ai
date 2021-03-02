#include "Base/Windows/Module.h"

#include <boost/filesystem.hpp>

#include "Base/Windows/WindowsError.h"
#include "Base/Windows/Apis.h"

namespace base
{
	namespace win
	{
		namespace filesystem = boost::filesystem;

		Module Module::Get(const char* name)
		{
			std::wstring nameW = Apis::Utf8ToWide(name);

			Module module(GetModuleHandleW(nameW.c_str()));
			if (module == nullptr)
				BASE_THROW(WindowsError());
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

		std::string Module::getPath() const
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = GetModuleFileNameW(m_module, buffer, BUFFER_SIZE - 1);
			if (ret == 0)
				BASE_THROW(WindowsError());
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				BASE_THROW(WindowsError());

			return Apis::WideToUtf8(buffer);
		}

		std::string Module::getDir() const
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = GetModuleFileNameW(m_module, buffer, BUFFER_SIZE - 1);
			if (ret == 0)
				BASE_THROW(WindowsError());
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				BASE_THROW(WindowsError());

			filesystem::path p(buffer);
			return Apis::WideToUtf8(p.remove_filename().wstring());
		}

		FARPROC Module::getProcAddress(const char* procName) const
		{
			FARPROC proc = GetProcAddress(m_module, procName);
			if (proc == nullptr)
				BASE_THROW(WindowsError());
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

		Module::operator HMODULE() const
		{
			return m_module;
		}
	}
}

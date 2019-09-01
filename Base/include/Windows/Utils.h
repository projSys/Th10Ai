#pragma once

#include "Base/Exception.h"

namespace win
{
	class Utils
	{
	public:
		static std::string GetErrorMessage(DWORD messageId);
		static std::string HresultToString(HRESULT hr);

		static std::string GetModulePath(HMODULE module = nullptr);
		static std::string GetModuleDir(HMODULE module = nullptr);
	};

#define THROW_WINDOWS_ERROR(error) \
	THROW_BASE_EXCEPTION(base::Exception() \
		<< base::err_no(error) \
		<< base::err_str(win::Utils::GetErrorMessage(error)))

#define THROW_DIRECTX_HRESULT(hr) \
	THROW_BASE_EXCEPTION(base::Exception() \
		<< base::err_hex(win::Utils::HresultToString(hr)) \
		<< base::err_str(win::Utils::GetErrorMessage(hr)))
}

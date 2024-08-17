#include "Base/Windows/Apis.h"

#include "Base/Exception/SystemError.h"

namespace base
{
	namespace win
	{
		fs::path Apis::GetModulePath(HMODULE module)
		{
			WCHAR buffer[BUFFER_SIZE] = {};
			DWORD ret = GetModuleFileNameW(module, buffer, BUFFER_SIZE - 1);
			if (ret == 0)
				Throw(SystemError(GetLastError()));
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				Throw(SystemError(ERROR_INSUFFICIENT_BUFFER));
			return fs::path(buffer);
		}

		fs::path Apis::GetModuleDir(HMODULE module)
		{
			return GetModulePath(module).parent_path();
		}
	}
}

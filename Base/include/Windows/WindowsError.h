#pragma once

#include "Base/Exception.h"

namespace win
{
	class WindowsError :
		public Exception
	{
	public:
		WindowsError(DWORD error,
			const char* func, const char* file, uint_t line);

		virtual void print(std::ostream& os) const override;

		DWORD getError() const;

	private:
		DWORD m_error;
	};

#define THROW_WINDOWS_ERROR(error) \
	throw win::WindowsError(error, __FUNCTION__, __FILE__, __LINE__)
}

#pragma once

#include "Base/Common.h"

//#include <experimental/source_location>

#include "Base/Types.h"

namespace base
{
	class SourceLocation
	{
	public:
		static void SetCurrent(
			const char* func, const char* file, uint_t line);
		static const SourceLocation& GetCurrent();

		SourceLocation();

		const char* getFunc() const;
		const char* getFile() const;
		uint_t getLine() const;

	private:
		thread_local static SourceLocation tls_current;

		const char* m_func;
		const char* m_file;
		uint_t m_line;
	};

#define STORE_SOURCE_LOCATION \
	SourceLocation::SetCurrent(__FUNCTION__, __FILE__, __LINE__)
}

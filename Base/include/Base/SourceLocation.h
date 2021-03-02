#pragma once

#include "Base/Common.h"

#include <ostream>

#include "Base/Types.h"

namespace base
{
	class SourceLocation
	{
	public:
		static void Set(const char* func, const char* file, uint_t line);
		static const SourceLocation& Get();

		SourceLocation();

		void printTo(std::ostream& os) const;

		const char* getFunc() const;
		const char* getFile() const;
		uint_t getLine() const;

	private:
		static thread_local SourceLocation stl_current;

		const char* m_func;
		const char* m_file;
		uint_t m_line;
	};

#define STORE_SOURCE_LOCATION \
	SourceLocation::Set(__FUNCTION__, __FILE__, __LINE__)
}

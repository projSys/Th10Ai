#pragma once

#include "Base/Common.h"

#include <ostream>

#include "Base/Types.h"

namespace base
{
	class SourceLocation
	{
	public:
		SourceLocation();
		SourceLocation(
			const char* func, const char* file, uint_t line);

		void print(std::ostream& os) const;

	private:
		const char* m_func;
		const char* m_file;
		uint_t m_line;
	};

#define BASE_CURRENT_LOCATION \
	base::SourceLocation(__FUNCTION__, __FILE__, __LINE__)
}

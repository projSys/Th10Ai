#pragma once

#include "Base/Common.h"

#include <ostream>
#include<boost/assert/source_location.hpp>

#include "Base/Types.h"
#include "Base/Singleton.h"

namespace base
{
	class SourceLocation :
		public ThreadLocal<SourceLocation>
	{
	public:
		SourceLocation(
			const char* func, const char* file, uint_t line);

		void print(std::ostream& os) const;

	private:
		const char* m_func;
		const char* m_file;
		uint_t m_line;
	};

#define BASE_CURRENT_LOCATION \
	base::SourceLocation sourceLocation(__FUNCTION__, __FILE__, __LINE__)
}

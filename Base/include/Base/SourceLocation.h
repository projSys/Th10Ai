#pragma once

#include "Base/Common.h"

#include <ostream>

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

		void printTo(std::ostream& os) const;

	private:
		const char* m_func;
		const char* m_file;
		uint_t m_line;
	};
}

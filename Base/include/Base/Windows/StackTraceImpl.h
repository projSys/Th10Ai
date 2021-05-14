#pragma once

#include "Base/Windows/Common.h"

#include <ostream>

namespace base
{
	namespace win
	{
		class StackTraceImpl
		{
		public:
			StackTraceImpl();
			explicit StackTraceImpl(DWORD skipped);

			void print(std::ostream& os) const;

		private:
			static constexpr DWORD BUFFER_SIZE = 128;

			PVOID m_frames[BUFFER_SIZE];
			WORD m_size;
		};
	}
}

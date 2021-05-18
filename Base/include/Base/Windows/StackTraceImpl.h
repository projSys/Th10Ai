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

			void clear();
			bool isValid() const;

		private:
			static constexpr DWORD BUFFER_SIZE = 128;

			bool m_valid;
			PVOID m_frames[BUFFER_SIZE];
			WORD m_size;
		};
	}
}

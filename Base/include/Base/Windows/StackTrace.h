#pragma once

#include "Base/Windows/Common.h"

#include <ostream>

#include "Base/Singleton.h"

namespace base
{
	namespace win
	{
		class StackTrace :
			public ThreadLocal<StackTrace>
		{
		public:
			StackTrace();

			void printTo(std::ostream& os) const;

		private:
			static constexpr DWORD BUFFER_SIZE = 128;

			PVOID m_frames[BUFFER_SIZE];
			WORD m_size;
		};
	}
}

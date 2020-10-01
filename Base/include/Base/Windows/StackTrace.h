#pragma once

#include "Base/Windows/Common.h"

#include <ostream>

#include "Base/Types.h"

namespace base
{
	namespace win
	{
		class StackTrace
		{
		public:
			explicit StackTrace(uint_t framesToSkip = 0);

			void printTo(std::ostream& os) const;

		private:
			static constexpr DWORD FRAME_MAX_COUNT = 128;
			PVOID m_frames[FRAME_MAX_COUNT];
			WORD m_frameCount;
		};
	}
}

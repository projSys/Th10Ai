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
			static constexpr DWORD FRAME_MAX_COUNT = 128;

			PVOID m_frames[FRAME_MAX_COUNT];
			WORD m_frameCount;
		};
	}
}

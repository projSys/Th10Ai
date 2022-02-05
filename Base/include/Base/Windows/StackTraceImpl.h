#pragma once

#include "Base/Windows/Common.h"

#include "Base/Streamable.h"

namespace base
{
	namespace win
	{
		class StackTraceImpl :
			public OStreamable
		{
		public:
			explicit StackTraceImpl(DWORD skips = 1);

			virtual void toStream(std::ostream& os) const override;

		private:
			static constexpr DWORD BUFFER_SIZE = 128;

			PVOID m_frames[BUFFER_SIZE];
			WORD m_size;
		};
	}
}

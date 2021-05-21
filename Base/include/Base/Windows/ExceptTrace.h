#pragma once

#include "Base/Windows/Common.h"

#include <mutex>

#include "Base/Singleton.h"

namespace base
{
	namespace win
	{
		class ExceptTrace :
			public Singleton<ExceptTrace>
		{
		public:
			ExceptTrace();

			void trace(EXCEPTION_POINTERS* info);

		private:
			std::mutex m_mutex;
		};
	}
}

#pragma once

#include "Base/Windows/Common.h"

#include <mutex>

namespace base
{
	namespace win
	{
		struct SharedData
		{
			HANDLE process;
			HANDLE thread;
			EXCEPTION_POINTERS* info;
			CONTEXT context;
		};

		class ExceptTrace
		{
		public:
			ExceptTrace();

			void trace(EXCEPTION_POINTERS* info);
			void f1();
			void f2();

		private:
			static constexpr DWORD64 BUFFER_SIZE = 256;

			std::mutex m_mutex;
			CONTEXT m_context;
			DWORD64 m_frames[BUFFER_SIZE];
			DWORD64 m_size;
		};

		extern ExceptTrace g_exceptTrace;
	}
}

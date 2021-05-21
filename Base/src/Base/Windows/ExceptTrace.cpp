#include "Base/Windows/ExceptTrace.h"

namespace base
{
	namespace win
	{
		struct ExceptInfo
		{
			EXCEPTION_POINTERS* info;
		};

		ExceptTrace::ExceptTrace() :
			Singleton(this)
		{
		}

		void ExceptTrace::trace(EXCEPTION_POINTERS* info)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
		}
	}
}

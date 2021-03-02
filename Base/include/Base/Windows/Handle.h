#pragma once

#include "Base/Windows/Common.h"

#include "Base/Types.h"

namespace base
{
	namespace win
	{
		class Handle
		{
		public:
			Handle();
			explicit Handle(HANDLE handle);
			virtual ~Handle();
			Handle(const Handle&) = delete;
			Handle(Handle&& other) noexcept;
			Handle& operator =(const Handle&) = delete;
			Handle& operator =(Handle&&) = delete;

			virtual bool wait(DWORD milliSeconds);

			bool operator ==(nullptr_t) const;
			bool operator !=(nullptr_t) const;
			operator HANDLE() const;
			HANDLE* operator &();

		protected:
			HANDLE m_handle;
		};
	}
}

#pragma once

#include "Base/Windows/Common.h"

#include "Base/Types.h"
#include "Base/String.h"

namespace base
{
	namespace win
	{
		class Module
		{
		public:
			static constexpr DWORD BUFFER_SIZE = 1024;

			static Module Get(const char* name);

			Module();
			explicit Module(HMODULE module);
			~Module();

			std::string getPath() const;
			std::string getDir() const;

			FARPROC getProcAddress(const char* procName) const;

			template <typename T>
			T getProcAddress(const char* procName) const
			{
				return reinterpret_cast<T>(getProcAddress(procName));
			}

			bool operator ==(nullptr_t) const;
			bool operator !=(nullptr_t) const;
			bool operator !() const;
			operator bool() const;
			operator HMODULE() const;

		protected:
			HMODULE m_module;
		};
	}
}

#pragma once

#include "Base/Common.h"

#include <concepts>
#include <type_traits>

namespace base
{
	template <typename T> requires std::invocable<T>
		class ScopeGuard
		{
		public:
			explicit ScopeGuard(T&& functor) :
				m_functor(std::forward<T>(functor))
			{
			}

			~ScopeGuard()
			{
				m_functor();
			}

		private:
			T m_functor;
		};

#define ON_SCOPE_EXIT(functor) ScopeGuard MACRO_CONCAT(scopeGuard, __LINE__)(functor)
}

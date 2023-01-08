#pragma once

#include "Base/Common.h"

#include <type_traits>
#include <concepts>

namespace base
{
	template <typename T>
		requires std::invocable<T>
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

		ScopeGuard(const ScopeGuard&) = delete;
		ScopeGuard(ScopeGuard&&) = delete;
		ScopeGuard& operator =(const ScopeGuard&) = delete;
		ScopeGuard& operator =(ScopeGuard&&) = delete;

	private:
		T m_functor;
	};

#define ON_SCOPE_EXIT(functor) ScopeGuard MACRO_CONCAT(scopeGuard, __LINE__)(functor)
}

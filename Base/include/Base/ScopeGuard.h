#pragma once

#include "Base/Common.h"

#include <type_traits>

namespace base
{
	template <typename T>
	class ScopeGuard
	{
	public:
		explicit ScopeGuard(T&& functor) :
			m_functor(forward<T>(functor)),
			m_isDismissed(false)
		{
		}

		~ScopeGuard()
		{
			if (!m_isDismissed)
				m_functor();
		}

		ScopeGuard(const ScopeGuard&) = delete;

		ScopeGuard(ScopeGuard&& other) :
			m_functor(move(other.m_functor)),
			m_isDismissed(other.m_isDismissed)
		{
			other.m_isDismissed = true;
		}

		ScopeGuard& operator =(const ScopeGuard&) = delete;
		ScopeGuard& operator =(ScopeGuard&&) = delete;

	private:
		T m_functor;
		bool m_isDismissed;
	};

	template <typename T>
	ScopeGuard<T> MakeScopeGuard(T&& functor)
	{
		return ScopeGuard<T>(forward<T>(functor));
	}

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)

#define ON_SCOPE_EXIT(functor) \
	auto SCOPEGUARD_LINENAME(scopeGuard, __LINE__) = base::MakeScopeGuard(functor)
}

#pragma once

namespace base
{
	template <typename T>
	class ScopeGuard
	{
	public:
		explicit ScopeGuard(T&& functor) :
			m_functor(std::forward<T>(functor)),
			m_dismissed(false)
		{
		}

		ScopeGuard(const ScopeGuard&) = delete;

		ScopeGuard(ScopeGuard&& other) :
			m_functor(std::move(other.m_functor)),
			m_dismissed(other.m_dismissed)
		{
			other.m_dismissed = true;
		}

		~ScopeGuard()
		{
			if (!m_dismissed)
				m_functor();
		}

		ScopeGuard& operator =(const ScopeGuard&) = delete;
		ScopeGuard& operator =(ScopeGuard&&) = delete;

	private:
		T m_functor;
		bool m_dismissed;
	};

	template <typename T>
	ScopeGuard<T> MakeScopeGuard(T&& functor)
	{
		return ScopeGuard<T>(std::forward<T>(functor));
	}

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)

#define ON_SCOPE_EXIT(functor) \
	auto SCOPEGUARD_LINENAME(scopeGuard, __LINE__) = base::MakeScopeGuard(functor)
}

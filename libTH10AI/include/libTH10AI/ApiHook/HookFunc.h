#pragma once

#include <MinHook.h>

namespace th
{
	template <typename T>
	class HookFunc
	{
	};

	template <typename Ret, typename... Args>
	class HookFunc<Ret(STDMETHODCALLTYPE *)(Args...)>
	{
	public:
		typedef Ret(STDMETHODCALLTYPE *FuncPtr_t)(Args...);

		HookFunc() :
			m_target(nullptr),
			m_original(nullptr)
		{
		}

		HookFunc(LPVOID target, LPVOID detour) :
			m_target(target),
			m_original(nullptr)
		{
			MH_STATUS status = MH_CreateHook(target, detour, reinterpret_cast<LPVOID*>(&m_original));
			if (status != MH_OK)
				THROW_BASE_EXCEPTION(Exception() << err_no(status) << err_str(MH_StatusToString(status)));

			status = MH_EnableHook(target);
			if (status != MH_OK)
			{
				MH_RemoveHook(target);
				THROW_BASE_EXCEPTION(Exception() << err_no(status) << err_str(MH_StatusToString(status)));
			}
		}

		HookFunc(const HookFunc&) = delete;

		HookFunc(HookFunc&& other) :
			m_target(other.m_target),
			m_original(other.m_original)
		{
			other.m_target = nullptr;
			other.m_original = nullptr;
		}

		~HookFunc()
		{
			if (m_target != nullptr)
			{
				MH_DisableHook(m_target);
				MH_RemoveHook(m_target);
			}
		}

		HookFunc& operator =(const HookFunc&) = delete;

		HookFunc& operator =(HookFunc&& other)
		{
			HookFunc(std::move(other)).swap(*this);
			return *this;
		}

		void swap(HookFunc& other)
		{
			std::swap(m_target, other.m_target);
			std::swap(m_original, other.m_original);
		}

		Ret operator ()(Args... args) const
		{
			return m_original(std::forward<Args>(args)...);
		}

	private:
		LPVOID m_target;
		FuncPtr_t m_original;
	};
}

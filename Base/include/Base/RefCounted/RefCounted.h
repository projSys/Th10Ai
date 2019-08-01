#pragma once

#include <atomic>
#include <cassert>

#include "Base/Type.h"

namespace base
{
	class RefCountedBase
	{
	public:
		// ���ü���Ĭ��Ϊ1�����ٶ�����ʱ�Ѿ���һ��sp�����˸ö���
		// ��ֹ�������������ʱsp���¶�������
		RefCountedBase() :
			m_strongCount(1),
			m_weakCount(1)
		{
		}

		virtual ~RefCountedBase() = default;

		void incStrongCount()
		{
			m_strongCount.fetch_add(1, std::memory_order_relaxed);
			incWeakCount();
		}

		void decStrongCount()
		{
			if (m_strongCount.fetch_sub(1, std::memory_order_release) == 1)
			{
				std::atomic_thread_fence(std::memory_order_acquire);
				destruct();
			}
			decWeakCount();
		}

		int64_t getStrongCount() const
		{
			return m_strongCount.load(std::memory_order_relaxed);
		}

		void incWeakCount()
		{
			m_weakCount.fetch_add(1, std::memory_order_relaxed);
		}

		void decWeakCount()
		{
			if (m_weakCount.fetch_sub(1, std::memory_order_release) == 1)
			{
				std::atomic_thread_fence(std::memory_order_acquire);
				deallocate();
			}
		}

		int64_t getWeakCount() const
		{
			return m_weakCount.load(std::memory_order_relaxed);
		}

	protected:
		virtual void destruct() = 0;
		virtual void deallocate() = 0;

		std::atomic_uint64_t m_strongCount;
		std::atomic<int64_t> m_weakCount;
	};



	template <typename T>
	class RefCounted :
		public RefCountedBase
	{
	public:
		RefCounted() :
			m_object(nullptr)
		{
		}

		T* get() const
		{
			return m_object;
		}

		void set(T* object)
		{
			m_object = object;
		}

	protected:
		virtual void destruct() override
		{
			assert(m_object != nullptr);
			m_object->~T();
		}

		virtual void deallocate() override
		{
			this->~RefCounted();
			delete[] this;
		}

		T* m_object;
	};
}

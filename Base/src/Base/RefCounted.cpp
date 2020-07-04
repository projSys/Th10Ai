#include "Base/Common.h"
#include "Base/RefCounted.h"

namespace base
{
	// 引用计数默认为1，防止构造过程中有临时sp/wp导致对象析构/内存释放
	RefCountedBase::RefCountedBase() :
		m_strongCount(1),
		m_weakCount(1)
	{
	}

	void RefCountedBase::incStrongCount()
	{
		m_strongCount.fetch_add(1, memory_order_relaxed);
	}

	void RefCountedBase::decStrongCount()
	{
		if (m_strongCount.fetch_sub(1, memory_order_release) == 1)
		{
			atomic_thread_fence(memory_order_acquire);
			destroy();
		}
	}

	uint_t RefCountedBase::getStrongCount() const
	{
		return m_strongCount.load(memory_order_relaxed);
	}

	void RefCountedBase::incWeakCount()
	{
		m_weakCount.fetch_add(1, memory_order_relaxed);
	}

	void RefCountedBase::decWeakCount()
	{
		if (m_weakCount.fetch_sub(1, memory_order_release) == 1)
		{
			atomic_thread_fence(memory_order_acquire);
			release();
		}
	}

	uint_t RefCountedBase::getWeakCount() const
	{
		return m_weakCount.load(memory_order_relaxed);
	}
}

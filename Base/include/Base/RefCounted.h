#pragma once

#include <atomic>

#include "Base/Types.h"

namespace base
{
	class RefCountedBase
	{
	public:
		RefCountedBase();
		virtual ~RefCountedBase() = default;

		void incStrongCount();
		void decStrongCount();
		uint_t getStrongCount() const;
		void incWeakCount();
		void decWeakCount();
		uint_t getWeakCount() const;

	private:
		virtual void destroy() noexcept = 0;
		virtual void release() noexcept = 0;

		atomic<uint_t> m_strongCount;
		atomic<uint_t> m_weakCount;
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

		void setObject(T* object)
		{
			m_object = object;
		}

	private:
		virtual void destroy() noexcept override
		{
			m_object->~T();
		}

		virtual void release() noexcept override
		{
			this->~RefCounted();
			delete[] this;
		}

		T* m_object;
	};
}

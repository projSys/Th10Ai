#pragma once

#include <cassert>

namespace base
{
#ifdef GetObject
#undef GetObject
#endif

	// сп╢М
	template <typename T>
	class sp
	{
	public:
		sp() :
			m_object(nullptr)
		{
		}

		template <typename U>
		explicit sp(U* object) :
			m_object(object)
		{
			if (m_object != nullptr)
			{
				RefCountedBase* refCounted = m_object->getRefCounted();
				assert(refCounted != nullptr);
				refCounted->incStrongCount();
			}
		}

		~sp()
		{
			if (m_object != nullptr)
			{
				RefCountedBase* refCounted = m_object->getRefCounted();
				refCounted->decStrongCount();
			}
		}

		sp(const sp& other) :
			m_object(other.m_object)
		{
			if (m_object != nullptr)
			{
				RefCountedBase* refCounted = m_object->getRefCounted();
				refCounted->incStrongCount();
			}
		}

		template <typename U>
		sp(const sp<U>& other) :
			m_object(other.get())
		{
			if (m_object != nullptr)
			{
				RefCountedBase* refCounted = m_object->getRefCounted();
				refCounted->incStrongCount();
			}
		}

		sp(sp&& other) :
			m_object(other.m_object)
		{
			other.m_object = nullptr;
		}

		template <typename U>
		sp(sp<U>&& other) :
			m_object(other.get())
		{
			other.m_object = nullptr;
		}

		sp& operator =(nullptr_t)
		{
			sp().swap(*this);
			return *this;
		}

		template <typename U>
		sp& operator =(U* object)
		{
			sp(object).swap(*this);
			return *this;
		}

		sp& operator =(const sp& other)
		{
			sp(other).swap(*this);
			return *this;
		}

		template <typename U>
		sp& operator =(const sp<U>& other)
		{
			sp(other).swap(*this);
			return *this;
		}

		sp& operator =(sp&& other)
		{
			sp(std::move(other)).swap(*this);
			return *this;
		}

		template <typename U>
		sp& operator =(sp<U>&& other)
		{
			sp(std::move(other)).swap(*this);
			return *this;
		}

		void swap(sp& other)
		{
			std::swap(m_object, other.m_object);
		}

		T* operator ->() const
		{
			return m_object;
		}

		T& operator *() const
		{
			return *m_object;
		}

		bool operator ==(nullptr_t) const
		{
			return m_object == nullptr;
		}

		bool operator !=(nullptr_t) const
		{
			return m_object != nullptr;
		}

		template <typename U>
		bool operator ==(const sp<U>& other) const
		{
			return m_object == other.get();
		}

		template <typename U>
		bool operator !=(const sp<U>& other) const
		{
			return m_object != other.get();
		}

		template <typename U>
		bool operator <(const sp<U>& other) const
		{
			return m_object < other.get();
		}

		template <typename U>
		bool operator <=(const sp<U>& other) const
		{
			return m_object <= other.get();
		}

		template <typename U>
		bool operator >(const sp<U>& other) const
		{
			return m_object > other.get();
		}

		template <typename U>
		bool operator >=(const sp<U>& other) const
		{
			return m_object >= other.get();
		}

	private:
		T* m_object;

		template <typename T>
		friend void SetObject(sp<T>& sp, T* object);

		template <typename T>
		friend T* GetObject(sp<T>& sp);
	};

	template <typename T>
	void SetObject(sp<T>& sp, T* object)
	{
		sp.m_object = object;
	}

	template <typename T>
	T* GetObject(sp<T>& sp)
	{
		return sp.m_object;
	}
}

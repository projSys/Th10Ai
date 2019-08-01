#pragma once

#include <cassert>

namespace base
{
	template <typename T>
	class wp
	{
	public:
		wp() :
			m_object(nullptr),
			m_refCounted(nullptr)
		{
		}

		template <typename U>
		explicit wp(const sp<U>& other) :
			m_object(other.get()),
			m_refCounted(nullptr)
		{
			if (m_object != nullptr)
			{
				m_refCounted = m_object->getRefCounted();
				m_refCounted->incWeakCount();
			}
		}

		wp(const wp& other) :
			m_object(other.m_object),
			m_refCounted(other.m_refCounted)
		{
			if (m_refCounted != nullptr)
				m_refCounted->incWeakCount();
		}

		template <typename U>
		wp(const wp<U>& other) :
			m_object(other.get()),
			m_refCounted(other.getRefCounted())
		{
			if (m_refCounted != nullptr)
				m_refCounted->incWeakCount();
		}

		wp(wp&& other) :
			m_object(other.m_object),
			m_refCounted(other.m_refCounted)
		{
			other.m_object = nullptr;
			other.m_refCounted = nullptr;
		}

		template <typename U>
		wp(wp<U>&& other) :
			m_object(other.get()),
			m_refCounted(other.getRefCounted())
		{
			other.m_object = nullptr;
			other.m_refCounted = nullptr;
		}

		~wp()
		{
			if (m_refCounted != nullptr)
				m_refCounted->decWeakCount();
		}

		wp& operator =(nullptr_t)
		{
			wp().swap(*this);
			return *this;
		}

		template <typename U>
		wp& operator =(const sp<U>& other)
		{
			wp(other).swap(*this);
			return *this;
		}

		wp& operator =(const wp& other)
		{
			wp(other).swap(*this);
			return *this;
		}

		template <typename U>
		wp& operator =(const wp<U>& other)
		{
			wp(other).swap(*this);
			return *this;
		}

		wp& operator =(wp&& other)
		{
			wp(std::move(other)).swap(*this);
			return *this;
		}

		template <typename U>
		wp& operator =(wp<U>&& other)
		{
			wp(std::move(other)).swap(*this);
			return *this;
		}

		void swap(wp& other)
		{
			std::swap(m_object, other.m_object);
			std::swap(m_refCounted, other.m_refCounted);
		}

		bool isValid() const
		{
			return m_refCounted != nullptr && m_refCounted->getStrongCount() > 0;
		}

		sp<T> lock() const
		{
			if (isValid())
				return sp<T>(m_object);
			else
				return sp<T>();
		}

		T* get() const
		{
			return m_object;
		}

		RefCountedBase* getRefCounted() const
		{
			return m_refCounted;
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
		bool operator ==(const wp<U>& other) const
		{
			return m_object == other.get();
		}

		template <typename U>
		bool operator !=(const wp<U>& other) const
		{
			return m_object != other.get();
		}

		template <typename U>
		bool operator <(const wp<U>& other) const
		{
			return m_object < other.get();
		}

		template <typename U>
		bool operator <=(const wp<U>& other) const
		{
			return m_object <= other.get();
		}

		template <typename U>
		bool operator >(const wp<U>& other) const
		{
			return m_object > other.get();
		}

		template <typename U>
		bool operator >=(const wp<U>& other) const
		{
			return m_object >= other.get();
		}

	private:
		T* m_object;
		RefCountedBase* m_refCounted;
	};
}

#pragma once

#include "Base/Common.h"

#include <cassert>

namespace base
{
	// сп╢М
	template <typename T>
	class wp
	{
	public:
		wp() :
			m_refCounted(nullptr),
			m_object(nullptr)
		{
		}

		template <typename U>
		explicit wp(const sp<U>& other) :
			m_refCounted(nullptr),
			m_object(other.get())
		{
			if (m_object != nullptr)
			{
				m_refCounted = m_object->getRefCounted();
				m_refCounted->incWeakCount();
			}
		}

		~wp()
		{
			if (m_refCounted != nullptr)
				m_refCounted->decWeakCount();
		}

		wp(const wp& other) :
			m_refCounted(other.m_refCounted),
			m_object(other.m_object)
		{
			if (m_refCounted != nullptr)
				m_refCounted->incWeakCount();
		}

		template <typename U>
		wp(const wp<U>& other) :
			m_refCounted(other.getRefCounted()),
			m_object(other.get())
		{
			if (m_refCounted != nullptr)
				m_refCounted->incWeakCount();
		}

		wp(wp&& other) :
			m_refCounted(other.m_refCounted),
			m_object(other.m_object)
		{
			other.m_refCounted = nullptr;
			other.m_object = nullptr;
		}

		template <typename U>
		wp(wp<U>&& other) :
			m_refCounted(other.getRefCounted()),
			m_object(other.get())
		{
			other.m_refCounted = nullptr;
			other.m_object = nullptr;
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
			wp(move(other)).swap(*this);
			return *this;
		}

		template <typename U>
		wp& operator =(wp<U>&& other)
		{
			wp(move(other)).swap(*this);
			return *this;
		}

		void swap(wp& other)
		{
			std::swap(m_refCounted, other.m_refCounted);
			std::swap(m_object, other.m_object);
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
		RefCountedBase* m_refCounted;
		T* m_object;
	};
}

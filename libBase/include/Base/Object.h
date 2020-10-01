#pragma once

#include "Base/Common.h"

#include "Base/Types.h"
#include "Base/RefCounted.h"
#include "Base/StrongPtr.h"
#include "Base/WeakPtr.h"

namespace base
{
	class Object
	{
	public:
		Object();
		virtual ~Object() = default;

		// 只要转调这个函数，其他混入类也能使用引用计数
		RefCountedBase* getRefCounted() const;

	private:
		thread_local static RefCountedBase* tls_refCounted;

		RefCountedBase* const m_refCounted;
		// 只要强引用存在，弱引用就不为0
		wp<Object> m_refBridge;

		template <typename T, typename... Args>
		friend sp<T> MakeObject(Args&&... args);
	};

	template <typename T, typename... Args>
	sp<T> MakeObject(Args&&... args)
	{
		byte_t* buffer = nullptr;
		RefCounted<T>* refCounted = nullptr;
		T* object = nullptr;
		try
		{
			buffer = new byte_t[sizeof(RefCounted<T>) + sizeof(T)];
			refCounted = new (buffer) RefCounted<T>();
			Object::tls_refCounted = refCounted;
			object = new (buffer + sizeof(RefCounted<T>)) T(std::forward<Args>(args)...);
			Object::tls_refCounted = nullptr;
			refCounted->setObject(object);
			// 引用计数已经默认为1，不需要调用sp的构造函数再加1
			sp<T> sp;
			SetObject(sp, object);
			return sp;
		}
		catch (...)
		{
			Object::tls_refCounted = nullptr;
			if (refCounted != nullptr)
				refCounted->~RefCounted<T>();
			if (buffer != nullptr)
				delete[] buffer;
			throw;
		}
	}
}

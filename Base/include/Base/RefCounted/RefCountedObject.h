#pragma once

#include "Base/Type.h"
#include "Base/RefCounted/RefCounted.h"

namespace base
{
	template <typename T>
	class sp;

	template <typename T>
	class wp;

	template <typename T, typename... Args>
	sp<T> MakeObject(Args&&... args);

	class RefCountedObject
	{
	public:
		RefCountedObject();
		virtual ~RefCountedObject() = default;

		// 只要转调这个函数，其他混入类也能使用引用计数
		RefCountedBase* getRefCounted() const;

	private:
		thread_local static RefCountedBase* tls_refCounted;
		RefCountedBase* m_refCounted;

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
			RefCountedObject::tls_refCounted = refCounted;
			object = new (buffer + sizeof(RefCounted<T>)) T(std::forward<Args>(args)...);
			RefCountedObject::tls_refCounted = nullptr;
			refCounted->setObject(object);
			// 引用计数已经默认为1，不需要调用sp的构造函数再加1
			sp<T> sp;
			sp.set(object);
			return sp;
		}
		catch (...)
		{
			RefCountedObject::tls_refCounted = nullptr;
			if (refCounted != nullptr)
				refCounted->~RefCounted<T>();
			if (buffer != nullptr)
				delete[] buffer;
			throw;
		}
	}
}

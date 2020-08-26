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

		// ֻҪת���������������������Ҳ��ʹ�����ü���
		RefCountedBase* getRefCounted() const;

	private:
		thread_local static RefCountedBase* tls_refCounted;

		RefCountedBase* const m_refCounted;
		// ֻҪǿ���ô��ڣ������þͲ�Ϊ0
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
			// ���ü����Ѿ�Ĭ��Ϊ1������Ҫ����sp�Ĺ��캯���ټ�1
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

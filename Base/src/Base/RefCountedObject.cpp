#include "Base/Common.h"
#include "Base/RefCounted/RefCountedObject.h"

namespace base
{
	thread_local RefCountedBase* RefCountedObject::tls_refCounted = nullptr;

	RefCountedObject::RefCountedObject() :
		m_refCounted(tls_refCounted)
	{
		// ֻ��ͨ��MakeObject�����ɶ���
		//assert(m_refCounted != nullptr);
	}

	RefCountedBase* RefCountedObject::getRefCounted() const
	{
		return m_refCounted;
	}
}

#include "Base/Common.h"
#include "Base/Object.h"

#include <cassert>

namespace base
{
	thread_local RefCountedBase* Object::tls_refCounted = nullptr;

	Object::Object() :
		m_refCounted(tls_refCounted)
	{
		// ֻ��ͨ��MakeObject�����ɶ���
		assert(m_refCounted != nullptr);
	}

	RefCountedBase* Object::getRefCounted() const
	{
		return m_refCounted;
	}
}

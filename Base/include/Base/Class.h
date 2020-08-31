#pragma once

#include "Base/Common.h"

#include <type_traits>

#include "Base/Types.h"

namespace base
{
	template <typename T>
	class Class
	{
	public:
		template <typename... Args>
		static T* Construct(void* address, Args&&... args)
		{
			return new (address) T(std::forward<Args>(args)...);
		}

		void Destruct(T* object)
		{
			if (object != nullptr)
				object->~T();
		}
	};
}

#pragma once

#include <cassert>

namespace base
{
	template <typename T>
	class Singleton
	{
	public:
		static T& GetInstance()
		{
			assert(s_instance != nullptr);
			return *s_instance;
		}

		explicit Singleton(T* instance)
		{
			assert(s_instance == nullptr);
			s_instance = instance;
		}

		~Singleton()
		{
			s_instance = nullptr;
		}

		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator =(const Singleton&) = delete;
		Singleton& operator =(Singleton&&) = delete;

	private:
		static T* s_instance;
	};

	template <typename T>
	T* Singleton<T>::s_instance = nullptr;
}

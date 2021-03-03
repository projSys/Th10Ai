#pragma once

#include "Base/Common.h"

#include <cassert>

namespace base
{
	template <typename T>
	class Singleton
	{
	public:
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

		static T& GetInstance()
		{
			assert(s_instance != nullptr);
			return *s_instance;
		}

	private:
		static T* s_instance;
	};

	template <typename T>
	T* Singleton<T>::s_instance = nullptr;

	template <typename T>
	class ThreadLocal
	{
	public:
		explicit ThreadLocal(T* instance)
		{
			assert(stl_instance == nullptr);
			stl_instance = instance;
		}

		~ThreadLocal()
		{
			stl_instance = nullptr;
		}

		static T& GetInstance()
		{
			assert(stl_instance != nullptr);
			return *stl_instance;
		}

	private:
		static thread_local T* stl_instance;
	};

	template <typename T>
	thread_local T* ThreadLocal<T>::stl_instance = nullptr;
}

#pragma once

#include "Base/Common.h"

#include <iosfwd>
#include <source_location>
//#include <stacktrace>
#include <type_traits>
//#include <string>
//#include <sstream>

#include "Base/Streamable.h"

namespace base
{
	std::ostream& operator <<(std::ostream& os, const std::source_location& sl);

	class Throwable :
		public OStreamable
	{
	public:
		virtual ~Throwable() = default;

		virtual void toStream(std::ostream& os) const override;
		virtual const std::source_location& getSourceLocation() const;

	private:
		static const std::source_location s_invalidSl;
	};

	template <typename T, typename Enable = void>
	class ThrowPacket
	{
	};

	template <typename T>
	class ThrowPacket<T, typename std::enable_if_t<std::is_base_of_v<Throwable, T>>> :
		public T
	{
	public:
		ThrowPacket(const T& exception,
			const std::source_location& sl) :
			T(exception),
			m_sl(sl)
		{
		}

		template <typename... Args>
		ThrowPacket(const std::source_location& sl,
			Args&&... args) :
			T(std::forward<Args>(args)...),
			m_sl(sl)
		{
		}

		virtual void toStream(std::ostream& os) const override
		{
			T::toStream(os);
			os << m_sl;
		}

		virtual const std::source_location& getSourceLocation() const override
		{
			return m_sl;
		}

	private:
		std::source_location m_sl;
	};

	//template <typename T>
	//class ThrowPacket<T, typename std::enable_if_t<std::is_base_of_v<std::exception, T>
	//	&& !std::is_base_of_v<Throwable, T>>> :
	//	public T
	//{
	//public:
	//	ThrowPacket(const T& exception,
	//		const std::source_location& sl) :
	//		T(exception),
	//		m_sl(sl)
	//	{
	//		std::ostringstream oss;
	//		oss << exception.what() << '\n';
	//		oss << sl;
	//		m_buffer = oss.str();
	//	}

	//	virtual const char* what() const override
	//	{
	//		return m_buffer.c_str();
	//	}

	//private:
	//	std::string m_buffer;
	//	std::source_location m_sl;
	//};

	template <typename T>
	inline void Throw(const T& exception,
		const std::source_location& sl = std::source_location::current())
	{
		throw ThrowPacket<T>(exception, sl);
	}

	template <typename T>
	class Thrower
	{
	public:
		explicit Thrower(
			const std::source_location& sl = std::source_location::current()) :
			m_sl(sl)
		{
		}

		template <typename... Args>
		void operator ()(Args&&... args) const
		{
			throw ThrowPacket<T>(m_sl, std::forward<Args>(args)...);
		}

	private:
		std::source_location m_sl;
	};
}

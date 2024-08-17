#pragma once

#include "Base/Common.h"

#include <iosfwd>
#include <source_location>
#include <type_traits>
//#include <exception>
//#include <string>
//#include <sstream>

#include "Base/Streamable.h"
#include "Base/Exception/StackTrace.h"

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
		virtual const StackTrace& getStackTrace() const;

	private:
		static const std::source_location s_invalidSl;
		static const StackTrace s_invalidSt;
	};

	template <typename T, typename Enable = void>
	class ThrowedPacket
	{
	};

	template <typename T>
	class ThrowedPacket<T, typename std::enable_if_t<std::is_base_of_v<Throwable, T>>> :
		public T
	{
	public:
		ThrowedPacket(const T& exception,
			const std::source_location& sl,
			StackTrace&& st) :
			T(exception),
			m_sl(sl),
			m_st(std::move(st))
		{
		}

		template <typename... Args>
		ThrowedPacket(const std::source_location& sl,
			StackTrace&& st,
			Args&&... args) :
			T(std::forward<Args>(args)...),
			m_sl(sl),
			m_st(std::move(st))
		{
		}

		virtual void toStream(std::ostream& os) const override
		{
			T::toStream(os);
			os << m_sl;
			os << m_st;
		}

		virtual const std::source_location& getSourceLocation() const override
		{
			return m_sl;
		}

		virtual const StackTrace& getStackTrace() const override
		{
			return m_st;
		}

	private:
		std::source_location m_sl;
		StackTrace m_st;
	};

	/*template <typename T>
	class ThrowedPacket<T, typename std::enable_if_t<std::is_base_of_v<std::exception, T>>> :
		public T
	{
	public:
		ThrowedPacket(const T& exception,
			const std::source_location& sl,
			StackTrace&& st) :
			T(exception),
			m_sl(sl),
			m_st(std::move(st))
		{
			std::ostringstream oss;
			oss << exception.what() << '\n';
			oss << m_sl;
			oss << m_st;
			m_buffer = oss.str();
		}

		virtual const char* what() const override
		{
			return m_buffer.c_str();
		}

	private:
		std::source_location m_sl;
		StackTrace m_st;
		std::string m_buffer;
	};*/

	template <typename T>
	inline void Throw(const T& exception,
		const std::source_location& sl = std::source_location::current(),
		StackTrace&& st = StackTrace())
	{
		throw ThrowedPacket<T>(exception, sl, std::move(st));
	}

	template <typename T>
	class Thrower
	{
	public:
		explicit Thrower(
			const std::source_location& sl = std::source_location::current(),
			StackTrace&& st = StackTrace()) :
			m_sl(sl),
			m_st(std::move(st))
		{
		}

		template <typename... Args>
		void operator ()(Args&&... args)
		{
			throw ThrowedPacket<T>(m_sl, std::move(m_st), std::forward<Args>(args)...);
		}

	private:
		std::source_location m_sl;
		StackTrace m_st;
	};
}

#pragma once

#include "Base/Common.h"

#include <iosfwd>
#include <source_location>
#include <type_traits>
//#include <exception>
//#include <string>
//#include <sstream>

#include "Base/Streamable.h"
//#include "Base/Exception/StackTrace.h"

namespace base
{
	std::ostream& operator <<(std::ostream& out, const std::source_location& sl);

	class Throwable :
		public OStreamable
	{
	public:
		virtual ~Throwable() = default;
		virtual void toStream(std::ostream& out) const override;
		virtual const std::source_location& getSourceLocation() const;
		//virtual const StackTrace& getStackTrace() const;

	private:
		static const std::source_location s_invalidSl;
		//static const StackTrace s_invalidSt;
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
			const std::source_location& sl/*,
			StackTrace&& st*/) :
			T(exception),
			m_sl(sl)/*,
			m_st(std::move(st))*/
		{
		}

		template <typename... Args>
		ThrowedPacket(const std::source_location& sl,
			//StackTrace&& st,
			Args&&... args) :
			T(std::forward<Args>(args)...),
			m_sl(sl)/*,
			m_st(std::move(st))*/
		{
		}

		virtual void toStream(std::ostream& out) const override
		{
			T::toStream(out);
			out << m_sl;
			//out << m_st;
		}

		virtual const std::source_location& getSourceLocation() const override
		{
			return m_sl;
		}

		//virtual const StackTrace& getStackTrace() const override
		//{
		//	return m_st;
		//}

	private:
		std::source_location m_sl;
		//StackTrace m_st;
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
			std::ostringstream out;
			out << exception.what() << '\n';
			out << m_sl;
			out << m_st;
			m_buffer = out.str();
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

	//template <typename T>
	//inline void Throw(const T& exception,
	//	const std::source_location& sl = std::source_location::current()/*,
	//	StackTrace&& st = StackTrace()*/)
	//{
	//	throw ThrowedPacket<T>(exception, sl/*, std::move(st)*/);
	//}

	//template <typename T>
	//class Thrower
	//{
	//public:
	//	explicit Thrower(
	//		const std::source_location& sl = std::source_location::current()/*,
	//		StackTrace&& st = StackTrace()*/) :
	//		m_sl(sl)/*,
	//		m_st(std::move(st))*/
	//	{
	//	}

	//	template <typename... Args>
	//	void operator ()(Args&&... args)
	//	{
	//		throw ThrowedPacket<T>(m_sl, /*std::move(m_st),*/ std::forward<Args>(args)...);
	//	}

	//private:
	//	std::source_location m_sl;
	//	//StackTrace m_st;
	//};
}

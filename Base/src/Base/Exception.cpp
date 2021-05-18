#include "Base/Exception.h"

#include <sstream>
#include <system_error>
#include <boost/exception/all.hpp>

namespace base
{
	thread_local SourceLocation tl_sourceLocation;
	thread_local StackTrace tl_stackTrace;

	Exception::Exception(const char* str) :
		runtime_error(str)
	{
	}

	Exception::Exception(const std::string& str) :
		runtime_error(str)
	{
	}

	void Exception::print(std::ostream& os) const
	{
		os << what() << '\n';
	}

	std::string PrintException()
	{
		std::ostringstream oss;
		try
		{
			throw;
		}
		catch (const Exception& ex)
		{
			ex.print(oss);
		}
		catch (const boost::exception& be)
		{
			oss << boost::diagnostic_information(be);
		}
		//catch (const std::system_error& ex)
		//{
		//	const std::error_code& ec = ex.code();
		//	oss << "Category: " << ec.category().name() << '\n'
		//		<< "Code: " << ec.value() << '\n'
		//		<< "Message: " << ec.message().c_str() << '\n'
		//		<< "What: " << ex.what() << '\n';
		//}
		catch (const std::exception& se)
		{
			oss << se.what() << '\n';
		}
		catch (...)
		{
			oss << "Unclassified exception.\n";
		}

		if (tl_sourceLocation.isValid())
		{
			tl_sourceLocation.print(oss);
			tl_sourceLocation.clear();
		}
		if (tl_stackTrace.isValid())
		{
			tl_stackTrace.print(oss);
			tl_stackTrace.clear();
		}
		return oss.str();
	}
}

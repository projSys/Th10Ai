#include "Base/Exception.h"

#include <sstream>
#include <boost/exception/all.hpp>

namespace base
{
	thread_local SourceLocation Exception::stl_sourceLocation;
	thread_local StackTrace Exception::stl_stackTrace;

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

		stl_sourceLocation.print(os);
		stl_stackTrace.print(os);
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
		catch (const std::exception& se)
		{
			oss << se.what() << '\n';
		}
		catch (...)
		{
			oss << "Unclassified exception.\n";
		}
		return oss.str();
	}
}

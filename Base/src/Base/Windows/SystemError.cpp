#include "Base/Windows/SystemError.h"

namespace base
{
	SystemError::SystemError(DWORD errorCode,
		const boost::source_location& loc) :
		system_error(errorCode, std::system_category()),
		Throwable(loc)
	{
	}

	void SystemError::print(std::ostream& os) const
	{
		//const std::error_code& ec = code();
		//os << "Category: " << ec.category().name() << '\n'
		//	<< "Code: " << ec.value() << '\n'
		//	<< "Message: " << ec.message().c_str() << '\n'
		//	<< "What: " << what() << '\n';
		os << what() << '\n';
		Throwable::print(os);
	}
}

#include "Base/Streamable.h"

namespace base
{
	std::istream& operator >>(std::istream& is,
		IStreamable& istreamable)
	{
		istreamable.fromStream(is);
		return is;
	}

	std::ostream& operator <<(std::ostream& os,
		const OStreamable& ostreamable)
	{
		ostreamable.toStream(os);
		return os;
	}

	std::istream& operator >>(std::istream& is,
		Streamable& streamable)
	{
		streamable.fromStream(is);
		return is;
	}

	std::ostream& operator <<(std::ostream& os,
		const Streamable& streamable)
	{
		streamable.toStream(os);
		return os;
	}
}

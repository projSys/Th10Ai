#pragma once

#include "Base/Common.h"

#include <iosfwd>

namespace base
{
	class IStreamable
	{
	public:
		virtual ~IStreamable() = default;
		virtual void fromStream(std::istream& is) = 0;
	};

	class OStreamable
	{
	public:
		virtual ~OStreamable() = default;
		virtual void toStream(std::ostream& os) const = 0;
	};

	class Streamable
	{
	public:
		virtual ~Streamable() = default;
		virtual void fromStream(std::istream& is) = 0;
		virtual void toStream(std::ostream& os) const = 0;
	};

	inline std::istream& operator >>(std::istream& is, IStreamable& istreamable)
	{
		istreamable.fromStream(is);
		return is;
	}

	inline std::ostream& operator <<(std::ostream& os, const OStreamable& ostreamable)
	{
		ostreamable.toStream(os);
		return os;
	}

	inline std::istream& operator >>(std::istream& is, Streamable& streamable)
	{
		streamable.fromStream(is);
		return is;
	}

	inline std::ostream& operator <<(std::ostream& os, const Streamable& streamable)
	{
		streamable.toStream(os);
		return os;
	}
}

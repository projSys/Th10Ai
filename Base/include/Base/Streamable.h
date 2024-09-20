#pragma once

#include "Base/Common.h"

#include <iosfwd>

namespace base
{
	class IStreamable
	{
	public:
		virtual ~IStreamable() = default;
		virtual void fromStream(std::istream& in) = 0;
	};

	class OStreamable
	{
	public:
		virtual ~OStreamable() = default;
		virtual void toStream(std::ostream& out) const = 0;
	};

	class Streamable
	{
	public:
		virtual ~Streamable() = default;
		virtual void fromStream(std::istream& in) = 0;
		virtual void toStream(std::ostream& out) const = 0;
	};

	inline std::istream& operator >>(std::istream& in, IStreamable& istreamable)
	{
		istreamable.fromStream(in);
		return in;
	}

	inline std::ostream& operator <<(std::ostream& out, const OStreamable& ostreamable)
	{
		ostreamable.toStream(out);
		return out;
	}

	inline std::istream& operator >>(std::istream& in, Streamable& streamable)
	{
		streamable.fromStream(in);
		return in;
	}

	inline std::ostream& operator <<(std::ostream& out, const Streamable& streamable)
	{
		streamable.toStream(out);
		return out;
	}
}

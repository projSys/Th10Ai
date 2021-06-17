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

	std::istream& operator >>(std::istream& is,
		IStreamable& istreamable);
	std::ostream& operator <<(std::ostream& os,
		const OStreamable& ostreamable);
	std::istream& operator >>(std::istream& is,
		Streamable& streamable);
	std::ostream& operator <<(std::ostream& os,
		const Streamable& streamable);
}

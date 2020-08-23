#pragma once

#include "Base/Common.h"

#include <type_traits>

#include "Base/Types.h"

namespace comm
{
	using namespace base;

	template <typename T>
	class EnumTraits
	{
	public:
		static constexpr T ENUMERATORS[] = {};
		static constexpr T NONE = T::NONE;
	};

	template <typename T, typename Enable = void>
	class Enum
	{
	};

	template <typename T>
	class Enum<T, std::enable_if_t<std::is_enum_v<T>>>
	{
	public:
		using enum_t = T;
		using underlying_t = typename std::underlying_type<enum_t>::type;
		using Traits = EnumTraits<enum_t>;

		static constexpr uint_t MAXCOUNT =
			sizeof(Traits::ENUMERATORS) / sizeof(Traits::ENUMERATORS[0]);

		Enum() :
			m_enumerator(Traits::NONE)
		{
		}

		Enum(enum_t enumerator) :
			m_enumerator(enumerator)
		{
		}

		//explicit Enum(underlying_t value)
		//{
		//}

		bool isNone() const
		{
			return m_enumerator == Traits::NONE;
		}

		operator underlying_t() const
		{
			return static_cast<underlying_t>(m_enumerator);
		}

	private:
		enum_t m_enumerator;
	};
}

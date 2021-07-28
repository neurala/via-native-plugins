/*
 * This file is part of Neurala SDK.
 * Copyright Neurala Inc. 2013-2021. All rights reserved.
 *
 * Except as expressly permitted in the accompanying License Agreement, if at all, (a) you shall
 * not license, sell, rent, lease, transfer, assign, distribute, display, host, outsource, disclose
 * or otherwise commercially exploit or make this source code available to any third party; (b) you
 * shall not modify, make derivative works of, disassemble, reverse compile or reverse engineer any
 * part of the SDK; (c) You shall not access the SDK in order to build a similar or competitive
 * product or service; (d) no part of the this source may be copied, reproduced, distributed,
 * republished, downloaded, displayed, posted or transmitted in any form or by any means, including
 * but not limited to electronic, mechanical, photocopying, recording or other means; and (e) any
 * future release, update, or other addition to functionality of the SDK shall be subject to the
 * terms of the accompanying License Agreement. You must reproduce, on all copies made by you or
 * for you, and must not remove, alter, or obscure in any way all proprietary rights notices
 * (including copyright notices) of Neurala Inc or its suppliers on or within the copies of the
 * SDK. Any sample code provided with the SDK and designated as such are for illustrative purposes
 * only and are not to be included in your applications.
 *
 * In cases when the accompanying License Agreement permits redistribution of this file, the above
 * notice shall be reproduced its entirety in every copy of a distributed version of this file.
 */

#ifndef NEURALA_META_ENUM_H
#define NEURALA_META_ENUM_H

#include <array>
#include <cstring>
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

#include "neurala/cppcompat/typetraits.h"

namespace neurala
{
/**
 * @defgroup NEURALA_META_ENUM Meta information about enum classes.
 *
 * @{
 */

/**
 * @brief Meta information about @c enum classes.
 *
 * This class needs to define an @c values static member variable with all the known enum values.
 * It also needs to define a @c fallbackValue for when a value is not listed in @c values as a
 * fallback.
 */
template<class Enum>
struct MetaEnum
{};

/**
 * @brief Registers @tparam Enum to support reflection via the associated @ref MetaEnum class.
 */
template<class Enum>
struct MetaEnumRegister
{
	using EnumType = Enum;
	using EnumIntType = std::underlying_type_t<Enum>;

	struct Entry
	{
		EnumIntType value{};
		const char* name{};

		constexpr Entry(EnumIntType v, const char* n) noexcept : value{v}, name{n} {}
		constexpr Entry(Enum v, const char* n) noexcept : value{EnumIntType(v)}, name{n} {}

		constexpr Enum enumValue() const noexcept { return static_cast<Enum>(value); }
	};

	static constexpr EnumIntType maxEnumValue = std::numeric_limits<EnumIntType>::max();

	static constexpr const auto fallbackValue = Entry{maxEnumValue, "unknown"};
};

/**
 * @brief Checks whether class @c T has @ref MetaEnum support.
 */
template<class T, class = std17::void_t<>>
struct hasMetaEnum : public std::false_type
{};

// NOTE: must still use std17::void_t since CUDA uses this file
template<class T>
struct hasMetaEnum<T, std17::void_t<decltype(MetaEnum<T>::values)>> : public std::true_type
{};

/**
 * @brief Creates a @c std::array with the enum values @p t, @p ts as created by
 *        @ref NEURALA_META_ENUM_ENTRY and @ref NEURALA_META_ENUM_ENTRY_ALT_NAME.
 *
 * @note This function is used to address the lack of inline static member variables in C++14 as
 *       explained in http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4424.pdf.
 */
template<class T, class... Ts>
constexpr auto
enumRegisterValues(T&& t, Ts&&... ts) noexcept
{
	return std::array<std::decay_t<T>, sizeof...(ts) + 1>{t, ts...};
}

/**
 * @brief Returns the name associated with enum value @p e.
 *
 * If @p e has no associated entry, it returns the fallback entry name.
 */
template<class Enum>
constexpr auto
enumToString(Enum e) noexcept
{
	using EnumType = std::decay_t<Enum>;
	using MetaEnumType = MetaEnum<EnumType>;

	for (const auto& entry : MetaEnumType::values)
	{
		if (e == entry.enumValue())
		{
			return entry.name;
		}
	}
	return MetaEnumType::fallbackValue.name;
}

/**
 * @brief Returns true if the enum is unknown
 */
template<class Enum>
constexpr bool
isEnumUnknown(Enum e) noexcept
{
	using EnumType = std::decay_t<Enum>;
	using MetaEnumType = MetaEnum<EnumType>;

	return e == MetaEnumType::fallbackValue.enumValue();
}

/**
 * @brief Returns the enum entry corresponding to @p s, otherwise the fallback entry.
 */
template<class Enum>
Enum
#if __cplusplus > 201402L
stringToEnum(std::string_view s) noexcept
#else
stringToEnum(const std::string& s) noexcept
#endif
{
	using MetaEnumType = MetaEnum<std::decay_t<Enum>>;

	for (const auto& entry : MetaEnumType::values)
	{
		if (s.compare(entry.name) == 0)
		{
			return entry.enumValue();
		}
	}
	return MetaEnumType::fallbackValue.enumValue();
}

/**
 * @brief Copies a <tt>std::pair<const char*, Enum></tt> to @p it.
 */
template<class Enum, class OutputIt>
constexpr void
enumCopyValues(OutputIt it)
{
	using MetaEnumType = MetaEnum<Enum>;

	for (const auto& entry : MetaEnumType::values)
	{
		*it++ = std::make_pair(entry.name, entry.enumValue());
	}
}

/**
 * @brief Copies a <tt>std::pair<const char*, Enum></tt> to @p it as @p Integral.
 */
template<class Enum, class Integral, class OutputIt>
constexpr void
enumCopyValuesAs(OutputIt it)
{
	using MetaEnumType = MetaEnum<Enum>;

	for (const auto& entry : MetaEnumType::values)
	{
		*it++ = std::make_pair(entry.name, Integral(entry.value));
	}
}

template<class Enum>
auto
operator<<(std::ostream& os, const Enum& e)
 -> std::enable_if_t<hasMetaEnum<Enum>::value, std::ostream&>
{
	return os << enumToString(e);
}

} // namespace neurala

/**
 * @defgroup NEURALA_META_ENUM_MACROS Neurala enum register commands
 *
 * @{
 */

/**
 * @brief Registers the value @p val from enum @p cl.
 */
#define NEURALA_META_ENUM_ENTRY(cl, val) \
	MetaEnumRegister<cl>::Entry { typename MetaEnumRegister<cl>::EnumIntType(cl::val), #val }

/**
 * @brief Registers the value @p val from enum @p cl with the alternative name @p name.
 */
#define NEURALA_META_ENUM_ENTRY_ALT_NAME(cl, val, name) \
	MetaEnumRegister<cl>::Entry { typename MetaEnumRegister<cl>::EnumIntType(cl::val), (name) }

/** @} */

/** @} */

#endif // NEURALA_META_ENUM_H

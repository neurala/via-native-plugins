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

#ifndef NEURALA_META_DETAIL_TYPE_NAME_H
#define NEURALA_META_DETAIL_TYPE_NAME_H

#include <cstddef>
#include <cstring>
#include <ostream>
#include <stdexcept>

namespace neurala
{
/**
 * @brief Compile-time string.
 *
 * @warning This is not a null-terminated string, so @ref size() is the only function that
 *          can convey it's length properly.
 */
class StaticString
{
public:
	using value_type = char;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using const_reference = const char&;
	using const_pointer = const char*;
	using const_iterator = const char*;

private:
	const char* const m_data{};
	const std::size_t m_size{};

public:
	/**
	 * @brief Constructs a new string from @p a.
	 */
	template<std::size_t N>
	constexpr StaticString(const char (&a)[N]) noexcept : m_data(a), m_size(N - 1)
	{}

	/**
	 * @brief Constructs a new string from @p a with length @p N.
	 */
	constexpr StaticString(const char* p, std::size_t N) noexcept : m_data(p), m_size(N) {}

	/**
	 * @brief Returns a pointer to the underlying array serving as character storage.
	 */
	constexpr const char* data() const noexcept { return m_data; }

	/**
	 * @brief Returns the number of elements in the string.
	 */
	constexpr std::size_t size() const noexcept { return m_size; }

	/// @copydoc size() const
	constexpr std::size_t length() const noexcept { return m_size; }

	/**
	 * @brief Returns an iterator to the first character of the string.
	 */
	constexpr const_iterator begin() noexcept { return m_data; }

	/// @copydoc begin()
	constexpr const_iterator begin() const noexcept { return m_data; }

	/// @copydoc begin()
	constexpr const_iterator cbegin() const noexcept { return m_data; }

	/**
	 * @brief Returns an iterator to the character following the last character of the string.
	 */
	constexpr const_iterator end() noexcept { return m_data + m_size; }

	/// @copydoc end()
	constexpr const_iterator end() const noexcept { return m_data + m_size; }

	/// @copydoc end()
	constexpr const_iterator cend() const noexcept { return m_data + m_size; }

	/**
	 * @brief Returns the character at specified location @p pos.
	 */
	constexpr char operator[](std::size_t pos) const
	{
		return pos < m_size ? m_data[pos] : throw std::out_of_range("StaticString");
	}
};

inline std::ostream&
operator<<(std::ostream& os, const StaticString& s)
{
	return os.write(s.data(), s.size());
}

/**
 * @brief Returns the type name of @p T as a @ref StaticString.
 */
template<class T>
constexpr auto
typeName() noexcept
{
	// see https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/20170989#20170989
	// for the idea behind this approach
#ifdef __clang__
	StaticString p = __PRETTY_FUNCTION__;
	StaticString prefix = "auto neurala::typeName() [T = ";
	StaticString suffix = "]";
#elif defined(__GNUC__)
	StaticString p = __PRETTY_FUNCTION__;
	StaticString prefix = "constexpr auto neurala::typeName() [with T = ";
	StaticString suffix = "]";
#elif defined(_MSC_VER)
	StaticString p = __FUNCSIG__;
	StaticString prefix = "auto __cdecl neurala::typeName<";
	StaticString suffix = ">(void) noexcept";
#else
# error "Unsupported compiler"
#endif

	return StaticString(p.data() + prefix.size(), p.size() - (prefix.size() + suffix.size()));
}

} // namespace neurala

#endif // NEURALA_META_DETAIL_TYPE_NAME_H

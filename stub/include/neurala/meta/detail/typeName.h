/*
 * Copyright Neurala Inc. 2013-2022
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:  The above copyright notice and this
 * permission notice (including the next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef NEURALA_META_DETAIL_TYPE_NAME_H
#define NEURALA_META_DETAIL_TYPE_NAME_H

#include <cstddef>
#include <cstring>
#include <ostream>
#include <stdexcept>

#include "neurala/exports.h"

namespace neurala
{
/**
 * @brief Compile-time string.
 *
 * @warning This is not a null-terminated string, so @ref size() is the only function that
 *          can convey it's length properly.
 */
class NEURALA_PUBLIC StaticString
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
	{ }

	/**
	 * @brief Constructs a new string from @p a with length @p N.
	 */
	constexpr StaticString(const char* p, std::size_t N) noexcept : m_data(p), m_size(N) { }

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

inline NEURALA_PUBLIC std::ostream&
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
	// see
	// https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/20170989#20170989
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
#error "Unsupported compiler"
#endif

	return StaticString(p.data() + prefix.size(), p.size() - (prefix.size() + suffix.size()));
}

} // namespace neurala

#endif // NEURALA_META_DETAIL_TYPE_NAME_H

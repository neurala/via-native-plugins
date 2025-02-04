/*
 * Copyright Neurala Inc. 2013-2025
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

#ifndef NEURALA_UTILS_STRING_H
#define NEURALA_UTILS_STRING_H

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <istream>
#include <limits>
#include <locale>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include "neurala/cppcompat/typetraits.h" // For std17::is_invocable in CUDA code
#include "neurala/exports.h"
#include "neurala/utils/getLine.h"

namespace neurala
{
namespace detail
{
/**
 * @brief Wrapper to allow @c char to be used with @ref stringify().
 */
class NEURALA_PUBLIC StringifyCharProxy
{
private:
	char m_c;

public:
	constexpr explicit StringifyCharProxy(char c) noexcept : m_c{c} { }

	constexpr operator char() const noexcept { return m_c; }

	constexpr std::size_t size() const noexcept { return 1u; }
};

/**
 * @brief Wrapper to allow <tt>const char*</tt> to be used with @ref stringify().
 */
class NEURALA_PUBLIC StringifyCharPtrProxy
{
private:
	const char* m_s;
	std::size_t m_size;

public:
	explicit StringifyCharPtrProxy(const char* s)
	 : m_s{s}, m_size{(s != nullptr ? std::strlen(s) : 0u)}
	{ }

	template<std::size_t N>
	constexpr explicit StringifyCharPtrProxy(const char (&s)[N]) : m_s{s}, m_size{N}
	{ }

	constexpr operator const char*() const noexcept { return m_s; }

	constexpr std::size_t size() const noexcept { return m_size; }
};

/**
 * @brief Converts @p t to a @c std::string.
 */
template<class T,
         std::enable_if_t<!std::is_same<typename std::decay<T>::type, std::string>::value
                           && !std::is_base_of<std::istream, typename std::decay<T>::type>::value,
                          int> = 0>
std::string
stringify(T&& t)
{
	std::ostringstream os;
	os << t;
	return os.str();
}

/**
 * @brief Specialization of @ref stringify(T&&) for @c std::string.
 */
template<class T, std::enable_if_t<std::is_same<typename std::decay<T>::type, std::string>::value, int> = 0>
auto
stringify(T&& t) -> decltype(std::forward<T>(t))
{
	return std::forward<T>(t);
}

/**
 * @brief Specialization of @ref stringify(T&&) for @c char.
 */
NEURALA_PUBLIC constexpr StringifyCharProxy
stringify(char c) noexcept
{
	return StringifyCharProxy{c};
}

/**
 * @brief Specialization of @ref stringify(T&&) for <tt>const char*</tt>.
 */
NEURALA_PUBLIC inline StringifyCharPtrProxy
stringify(const char* c) noexcept
{
	return StringifyCharPtrProxy{c};
}

/**
 * @brief Specialization of @ref stringify(T&&) for <tt>std::istream</tt>.
 */
NEURALA_PUBLIC inline std::string
stringify(std::istream& is) noexcept
{
	std::string s;
	char buffer[4096];
	while (is.read(buffer, sizeof(buffer)))
	{
		s.append(buffer, sizeof(buffer));
	}
	s.append(buffer, is.gcount());
	return s;
}

/**
 * @ref toString() implementation.
 *
 * This function relies on parameter pack expansion to reserve the space required for a
 * @c std::string object and append the individual objects to it.
 */
template<class... T>
std::string
toStringImpl(T&&... t)
{
	using array_type = int[];

	// find size
	std::string::size_type size{};
	static_cast<void>(array_type{(size += t.size(), 0)...});

	// append to string
	std::string s;
	s.reserve(size);
	static_cast<void>(array_type{(s += std::forward<T>(t), 0)...});

	return s;
}

} // namespace detail

/**
 * @brief Converts the variadic pack @p t to a @c std::string.
 */
template<class... T>
std::string
toString(T&&... t)
{
	return detail::toStringImpl(detail::stringify(std::forward<T>(t))...);
}

/**
 * @brief Converts @p t to a @c std::string.
 */
template<class T>
std::string
toString(T&& t)
{
	return std::string{detail::stringify(std::forward<T>(t))};
}

/**
 * @brief Converts @p val to a string with the specified decimal precision.
 *
 * @param val Value to convert
 * @param precision number of decimal places to the right to return.  If @p precision is < 0, the
 * maximum allowed precision (without the number changing) for that data type will be used.
 *
 * @note The number will be rounded when it is printed.  If the trailing number is >= 5, the number
 * will be rounded up. For example:
 *
 * @code
 * neurala::toStringWithPrecision(0.987654f, 0) == "1"
 * neurala::toStringWithPrecision(0.987654f, 1) == "1.0"
 * neurala::toStringWithPrecision(0.987654f, 4) == "0.9877"
 * neurala::toStringWithPrecision(0.987654f, 5) == "0.98765"
 * @endcode
 */
template<class T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
std::string
toStringWithPrecision(T val, int precision)
{
	std::stringstream stream;

	if (precision < 0)
	{
		// std::fixed is needed so that trailing zeros arent automatically truncated
		stream << std::fixed << std::setprecision(std::numeric_limits<T>::digits10) << val;
	}
	else
	{
		stream << std::fixed << std::setprecision(precision) << val;
	}

	return stream.str();
}

/**
 * @brief Converts @p val to a string stripping any trailing zeros, thus "0.9000000" can be returned
 * as "0.9".
 */
NEURALA_PUBLIC inline std::string
toStringRemoveTrailingZeroes(float val)
{
	auto str = toString(val);
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	return str;
}

/**
 * @brief Similar to std::strcmp, except each char is converted to uppercase before being compared.
 * So, for example, "imagenet" and "IMAGENET" will be considered equal.
 *
 * @return 0 if lhs and rhs compare equal, -1 if lhs appears before rhs in lexicographical order,
 * and 1 if lhs appears after rhs in lexicographical order.
 */
NEURALA_PUBLIC inline int
strcmpCaseInsensitive(const char* lhs, const char* rhs)
{
	while (*lhs | *rhs)
	{
		auto lhsUppercase = std::toupper(*lhs++, std::locale());
		auto rhsUppercase = std::toupper(*rhs++, std::locale());

		if (lhsUppercase < rhsUppercase)
		{
			return -1;
		}
		if (lhsUppercase > rhsUppercase)
		{
			return 1;
		}
	}
	return 0;
}

namespace detail
{
using std17::is_invocable;
/**
 * @copydoc split(const std::string&,const char,Output&&)
 */
template<class UnaryFunction>
std::enable_if_t<is_invocable<UnaryFunction, std::string&&>::value, void>
split(const std::string& word, const char sep, UnaryFunction&& output)
{
	std::istringstream iss(word);
	std::string token;
	while (getLine(iss, token, sep))
	{
		std::forward<UnaryFunction>(output)(std::move(token));
	}
}

/**
 * @copydoc split(const std::string&,const char,Output&&)
 */
template<class OutputIterator>
std::enable_if_t<std::is_same<typename std::iterator_traits<OutputIterator>::iterator_category,
                              std::output_iterator_tag>::value,
                 void>
split(const std::string& word, const char sep, OutputIterator result)
{
	split(word, sep, [&](auto&& token) { *result++ = std::move(token); });
}

} // namespace detail

/**
 * @brief Separate a string using a separator character
 *
 * @warning When @p word ends with separator @p sep, the token list will not end with an empty
 * element.
 *
 * @param word String to split
 * @param sep Separator character
 * @param output Token consumer object
 */
template<class Output>
void
split(const std::string& word, const char sep, Output&& output)
{
	detail::split(word, sep, std::forward<Output>(output));
}

namespace detail
{
inline char
toLower(char ch)
{
	return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

inline char
toUpper(char ch)
{
	return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}
} // namespace detail

NEURALA_PUBLIC inline std::string
toLowerCase(const std::string& str)
{
	std::string result{str};
	std::transform(result.begin(), result.end(), result.begin(), detail::toLower);
	return result;
}

NEURALA_PUBLIC inline std::string
toUpperCase(const std::string& str)
{
	std::string result{str};
	std::transform(result.begin(), result.end(), result.begin(), detail::toUpper);
	return result;
}

NEURALA_PUBLIC inline bool
endsWith(std::string_view str, std::string_view suffix)
{
	return str.size() >= suffix.size()
	       && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

template<typename Container>
bool
endsWithAnyOf(std::string_view str, const Container& suffixes)
{
	return std::any_of(std::begin(suffixes), std::end(suffixes), [&str](std::string_view suffix) {
		return endsWith(str, suffix);
	});
}

} // namespace neurala

#endif // NEURALA_UTILS_STRING_H

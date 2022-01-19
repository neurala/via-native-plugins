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

#ifndef NEURALA_UTILS_VERSION_H
#define NEURALA_UTILS_VERSION_H

#include <cstdint>
#include <ostream>
#include <string>

#ifdef __GLIBCXX__
// Undefine major and minor macros provided by glibc
#undef major
#undef minor
#endif // __GLIBCXX__

namespace neurala
{
/**
 * @brief Keeps track of version number.
 *
 * Major and minor numbers are required, but revision is optional.  If revision is not set (or is
 * set to a negative number) it will be ignored in comparisons and printing.
 */
class Version
{
	std::uint16_t m_major{};
	std::uint16_t m_minor{};
	/// Revision version is optional and will be ignored if set to negative values.
	std::int16_t m_revision = -1;

public:
	Version() = default;

	/**
	 * @brief Constructs a Version from integers.  A @c -1 value for the revision means that the
	 * revision is not used.
	 */
	constexpr explicit Version(std::uint16_t maj, std::uint16_t min, std::int16_t rev = -1) noexcept
	 : m_major{maj}, m_minor{min}, m_revision{rev}
	{ }

	/**
	 * @brief Constructs a Version from a string.
	 *
	 * @param str String to parse version from.  Should be in format "#.#" or "#.#.#" but may have
	 * any number of non numeric characters (or whitespace) before or after.
	 *
	 * @returns default Version(0u, 0u, -1) if string wasn't able to be split and parsed
	 */
	explicit Version(const std::string& str);

	/**
	 * @brief Returns the major version.
	 */
	constexpr std::uint16_t major() const noexcept { return m_major; }

	/**
	 * @brief Returns the major version.
	 */
	constexpr std::uint16_t minor() const noexcept { return m_minor; }

	/**
	 * @brief Returns the major version.
	 */
	constexpr std::uint16_t revision() const noexcept { return m_revision; }

	/**
	 * @brief Returns if the revision is valid.
	 */
	constexpr bool hasRevision() const noexcept { return m_revision > -1; }

	/**
	 * @brief Returns a string representation of this version object.
	 */
	std::string toString() const;

	/**
	 * @brief Increments the version number.
	 *
	 * E.g.:
	 * 1.2.0  + 1.1.-1   = 2.3.0
	 * 1.2.-1 + 1.1.8    = 2.3.8
	 */
	Version& operator+=(const Version& ver) noexcept;

	/**
	 * @brief Serializes this object to @p archive.
	 */
	template<class Archive>
	void serialize(Archive& archive);

	friend std::ostream& operator<<(std::ostream&, const Version&);

// NOTE:20210927:jgerity:SWIG does not support `friend constexpr` (https://github.com/swig/swig/issues/2079)
#ifndef SWIG
	/**
	 * @brief Returns @c true if @p x and @p y major, minor, and revision match.
	 *
	 * If both objects have negative revision values, revision number will not be compared.
	 */
	friend constexpr bool operator==(const Version& x, const Version& y) noexcept
	{
		return x.major() == y.major() && x.minor() == y.minor()
		       && (!x.hasRevision() && !y.hasRevision() ? true : x.revision() == y.revision());
	}

	friend constexpr bool operator!=(const Version& x, const Version& y) noexcept { return !(x == y); }

	/**
	 * @brief Returns @c true if major, minor, or revision of @p x is less than @p y.
	 *
	 * If both objects have negative revision values, revision number will not be compared.
	 *
	 * E.g.:
	 * 1.1 < 1.2       = true
	 * 1.1.-1 < 1.1.-5 = false
	 * 1.1.-1 < 1.1.-5 = false
	 */
	friend constexpr bool operator<(const Version& x, const Version& y) noexcept
	{
		return x.major() < y.major() || (x.major() == y.major() && x.minor() < y.minor())
		       || (x.major() == y.major() && x.minor() == y.minor()
		           && (!x.hasRevision() && !y.hasRevision() ? false : x.revision() < y.revision()));
	}

	friend constexpr bool operator<=(const Version& x, const Version& y) noexcept
	{
		return x == y || x < y;
	}

	friend constexpr bool operator>(const Version& x, const Version& y) noexcept { return y < x; }

	friend constexpr bool operator>=(const Version& x, const Version& y) noexcept { return y <= x; }

	/// @copydoc Version::operator+=(const Version&)
	friend Version operator+(Version v1, const Version& v2) noexcept;
#endif // SWIG
};

} // namespace neurala

#endif // NEURALA_UTILS_VERSION_H

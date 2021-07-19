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

#ifndef NEURALA_UTILS_VERSION_H
#define NEURALA_UTILS_VERSION_H

#include <cstdint>
#include <ostream>
#include <string>

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
	{}

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
		return x.major() < y.major()
		       || (x.major() == y.major() && x.minor() < y.minor())
		       || (x.major() == y.major()
		           && x.minor() == y.minor()
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

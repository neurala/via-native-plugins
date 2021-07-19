
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

#ifndef NEURALA_META_SOURCE_LOCATION_H
#define NEURALA_META_SOURCE_LOCATION_H

/**
 * @def NEURALA_CURRENT_FUNCTION
 * Returns a c-string of the current function.
 */
#if defined(__GNUC__)
# define NEURALA_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
# define NEURALA_CURRENT_FUNCTION __FUNCSIG__
#else
# define NEURALA_CURRENT_FUNCTION __func__
#endif

namespace neurala
{
/**
 * @brief Container for function name, line, and file.
 *
 * This is modeled after
 * https://en.cppreference.com/w/cpp/experimental/source_location/source_location
 */
class SourceLocation
{
	const char* m_function{};
	int m_line{};

public:
	SourceLocation() = default;

	constexpr SourceLocation(const char* function, int line) noexcept
	 : m_function{function}, m_line{line}
	{}

	constexpr const char* function() const noexcept { return m_function; }

	constexpr int line() const noexcept { return m_line; }
};

} // namespace neurala

/**
 * @def NEURALA_CURRENT_SOURCE_LOCATION
 * Returns the current @ref neurala::SourceLocation.
 */
#define NEURALA_CURRENT_SOURCE_LOCATION() neurala::SourceLocation(NEURALA_CURRENT_FUNCTION, __LINE__)

#endif // NEURALA_META_SOURCE_LOCATION_H

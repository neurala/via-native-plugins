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

#ifndef NEURALA_ERROR_EXCEPTION_H
#define NEURALA_ERROR_EXCEPTION_H

#include <exception>
#include <string>

#include "neurala/error/ErrorCode.h"
#include "neurala/exports.h"
#include "neurala/meta/SourceLocation.h"
#include "neurala/utils/string.h"

namespace neurala
{

/**
 * @brief Neurala exception class.
 */
class NEURALA_PUBLIC Exception : public std::exception
{
	SourceLocation m_sourceLocation;
	/// Error information
	std::error_code m_errorCode;
	/// Error message
	std::string m_errorMessage;


	/**
	 * @brief Logs the exception.
	 */
	void logException() noexcept;

	/**
	 * @brief Creates an error message or an empty in case of an exception.
	 */
	template<class... T>
	static std::string toStringNoExcept(T&&... t) noexcept
	{
		try
		{
			return toString(std::forward<T>(t)...);
		}
		catch (...)
		{ }

		return {};
	}

public:
	Exception() = default;

	/**
	 * @brief Creates an exception.
	 *
	 * @param sourceLocation source location where the exception occurred
	 * @param error          The error code to throw
	 */
	explicit Exception(const SourceLocation& sourceLocation, const std::error_code& errorCode) noexcept;

	/**
	 * @brief Creates an exception.
	 *
	 * @param sourceLocation source location where the exception occurred
	 * @param t              arguments that create the explanation of what happened
	 */
	template<class... T>
	explicit Exception(const SourceLocation& sourceLocation,
	                   const std::error_code& errorCode,
	                   T&&... t) noexcept
	 : m_sourceLocation{sourceLocation},
	   m_errorCode{errorCode},
	   m_errorMessage{toStringNoExcept(std::forward<T>(t)...,
	                                   " (",
	                                   m_sourceLocation.function(),
	                                   ':',
	                                   m_sourceLocation.line(),
	                                   ')')}
	{
		logException();
	}

	/**
	 * @brief Returns the exception explanation message.
	 */
	const char* what() const noexcept override;

	/**
	 * @brief Returns the error code associated to the exception
	 */
	const std::error_code& code() const noexcept;
};

/**
 * @brief Prints the function name and line from @p sourceLocation, the name of the error and the
 * description and aborts the program.
 *
 * @note This function is typically used when exceptions cannot be used and an unrecoverable error
 *       has been encountered.
 */
NEURALA_PUBLIC void catastrophicError(const SourceLocation& sourceLocation,
                                      const char* errorName,
                                      const char* errorDescription) noexcept;

} // namespace neurala

/**
 * @def NEURALA_THROW_EXCEPTION(...)
 * Throws a @ref neurala::Exception with the caller function name and a message explaining the
 * exception.
 */
#ifdef HIDE_NEURALA_EXCEPTION_STRINGS
#define NEURALA_THROW_EXCEPTION(...) throw neurala::Exception()
#else
#define NEURALA_THROW_EXCEPTION(...)                           \
	throw neurala::Exception(NEURALA_CURRENT_SOURCE_LOCATION(),   \
	                         neurala::B4BError::GENERIC_EXCEPTION, \
	                         __VA_ARGS__);
#endif

#define NEURALA_THROW_ERROR(error) \
	throw neurala::Exception(NEURALA_CURRENT_SOURCE_LOCATION(), std::error_code{error});

/**
 * @def NEURALA_GUARD(cond, ...)
 * Throws a @ref neurala::Exception with the provided error message if @p cond is @c false.
 */
#define NEURALA_GUARD(cond, ...)         \
	do                                      \
	{                                       \
		if (!(cond))                           \
		{                                      \
			NEURALA_THROW_EXCEPTION(__VA_ARGS__); \
		}                                      \
	} while (false)

/**
 * @def NEURALA_GUARD_NO_THROW(cond, ...)
 * Aborts the program with the provided error message if @p cond is @c false.
 */
#define NEURALA_GUARD_NO_THROW(cond, ...)                                                \
	do                                                                                      \
	{                                                                                       \
		if (!(cond))                                                                           \
		{                                                                                      \
			neurala::catastrophicError(NEURALA_CURRENT_SOURCE_LOCATION(), nullptr, #__VA_ARGS__); \
		}                                                                                      \
	} while (false)

#endif // NEURALA_ERROR_EXCEPTION_H

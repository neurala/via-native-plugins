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

#ifndef NEURALA_ERROR_ASSERT_H
#define NEURALA_ERROR_ASSERT_H

#ifdef NEURALA_ENABLE_ASSERTS

#include "neurala/config/os.h"
#include "neurala/meta/SourceLocation.h"
#include "neurala/exports.h"

namespace neurala
{

/**
 * @brief Prints @p expr, @p file, function name, and source code line to standard error and calls
 *        @c std::abort().
 *
 * @param msg            message
 * @param file           file
 * @param sourceLocation source location
 */
NEURALA_PUBLIC void
assertionFailed(const char* msg, const char* file, const SourceLocation& sourceLocation);

} // namespace neurala

/**
 * @def NEURALA_ASSERT(condition)
 * If @c NDEBUG is not defined or @c NEURALA_ENABLE_ASSERTS is defined, it checks @p condition and
 * if it is @c false, it outputs diagnostic information on the standard error output and calls
 * @c std::abort().
 */

/**
 * @def NEURALA_ASSERT_MSG(condition, msg)
 * If @c NDEBUG is not defined or @c NEURALA_ENABLE_ASSERTS is defined, it checks @p expr and if it
 * is @c false, it outputs @p msg and diagnostic information on the standard error output and calls
 * @c std::abort().
 */

# define NEURALA_ASSERT_MSG(condition, msg) \
 (!!(condition) ? ((void)0) : ::neurala::assertionFailed(msg, __FILE__, NEURALA_CURRENT_SOURCE_LOCATION()))

# define NEURALA_ASSERT(condition) NEURALA_ASSERT_MSG((condition), #condition)

#else // NEURALA_ENABLE_ASSERTS

# define NEURALA_ASSERT(condition) ((void)0)
# define NEURALA_ASSERT_MSG(condition, msg) ((void)0)

#endif // NEURALA_ENABLE_ASSERTS

#endif // NEURALA_ERROR_ASSERT_H

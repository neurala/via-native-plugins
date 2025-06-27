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

#ifndef NEURALA_UTILS_DETAIL_DEMANGLE_H
#define NEURALA_UTILS_DETAIL_DEMANGLE_H

#include <string>
#include <string_view>

#include "neurala/exports.h"

namespace neurala
{
/**
 * @brief Returns the demangled name of @p name as @c std::string
 *
 * @note If @p name cannot be demangled (e.g. already demangled), it is returned as is
 * @note If an allocation error occurs, @c std::bad_alloc is thrown
 */
NEURALA_PUBLIC std::string demangle(std::string_view name);

} // namespace neurala

#endif // NEURALA_UTILS_DETAIL_DEMANGLE_H

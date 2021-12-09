/*
 * Copyright (c) Neurala Inc. 2013-2021
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

#ifndef NEURALA_ERROR_ERROR_CODE_H
#define NEURALA_ERROR_ERROR_CODE_H

#include <system_error>

#include "neurala/exports.h"
#include "neurala/meta/enum.h"

namespace neurala
{
struct B4BError final
{
	static constexpr B4BError ok() { return {0}; }
	static constexpr B4BError unknown() { return {1}; }
	static constexpr B4BError notImplemented() { return {2}; }
	static constexpr B4BError genericError() { return {3}; }
	static constexpr B4BError invalidParameter() { return {4}; }
	static constexpr B4BError unsupportedAction() { return {5}; }

	constexpr operator int() const noexcept { return m_value; }

	int m_value;
};

NEURALA_PUBLIC const std::error_category& b4bCategory() noexcept;

/**
 * @brief Generate and error code from arguments
 */
NEURALA_PUBLIC std::error_code make_error_code(B4BError error) noexcept; // NOLINT

/**
 * @brief Generate and error category from arguments
 */
NEURALA_PUBLIC std::error_condition make_error_condition(B4BError error) noexcept; // NOLINT

} // namespace neurala

namespace std
{
template<>
struct is_error_code_enum<neurala::B4BError> : public true_type
{ };
} // namespace std

#endif // NEURALA_ERROR_ERROR_CODE_H

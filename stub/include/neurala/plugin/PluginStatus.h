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

#ifndef NEURALA_PLUGIN_PLUGIN_STATUS_H
#define NEURALA_PLUGIN_PLUGIN_STATUS_H

#include <system_error>

#include "neurala/exports.h"
#include "neurala/plugin/PluginBindings.h"

namespace neurala
{
/**
 * @brief Plugin status codes.
 */
enum class PluginStatus
{
	success,
	unknown,
	wrongVersion,
	invalidName,
	alreadyRegistered
};

/// @brief Definition of the plugin status error domain
NEURALA_PUBLIC const std::error_category& pluginStatusCategory() noexcept;

/**
 * @brief Creates a std::error_condition from a @p neurala::PluginStatus
 * @param status The code to convert
 */
NEURALA_PUBLIC std::error_condition make_error_condition(PluginStatus status) noexcept; // NOLINT

/**
 * @brief Creates a std::error_condition from a @p neurala::PluginStatus
 *
 * @note While @p neurala::PluginStatus can be used as a std::error_code, you are encouraged to
 * create your own error codes and define equivalence relationship with this status.
 *
 * @param status The code to convert
 */
NEURALA_PUBLIC std::error_code make_error_code(PluginStatus status) noexcept; // NOLINT

} // namespace neurala

namespace std
{
template<>
struct is_error_condition_enum<neurala::PluginStatus> : true_type
{ };
} // namespace std

#endif // NEURALA_PLUGIN_PLUGIN_STATUS_H
